#pragma once
#include <mutex>
#include <queue>
#include <deque>
#include <condition_variable>
#include <scl/exceptions/NonNullViolation.h>

namespace scl{
	namespace async{
		template <class, class, template<class> class, class>
		class Channel;

		namespace details{
			template <class>
			class ChannelSender;

			template <class>
			class ChannelReceiver;

			/**
			 * Traits for the implementation of the transport of information in a scl::async::Channel
			 * @tparam Channel being the channel type
			 */
			template <class Channel>
			struct channel_transport_traits{
				using channel_type = Channel;
				using sender_type = typename channel_type::sender_type;
				using receiver_type = typename channel_type::receiver_type;
				using type = std::pair<sender_type, receiver_type>;

				static type factory(sender_type sender, receiver_type receiver){
					return std::make_pair(std::move(sender), std::move(receiver));
				}
			};

			/**
			 * Traits for actors that interact with the channel
			 * @tparam ChannelActor being the type of the actor
			 */
			template <class ChannelActor>
			struct channel_actor_traits{
				/**
				 * @typedef actor_type
				 * The type of the actor
				 */
				using actor_type = ChannelActor;

				/**
				 * @typedef channel_type
				 * The type of the channel
				 */
				using channel_type = typename actor_type::channel_type;

				/**
				 * @typedef lock_type
				 * The type of lock used by the channel
				 */
				using lock_type = typename channel_type::lock_type;

				/**
				 * @typedef guard_type
				 * @tparam L being the type of lock
				 * The type of guard used to protect the lock
				 */
				template <class L>
				using guard_type = typename channel_type::guard_type<L>;

				/**
				 * @typedef value_type
				 * The type of values transmitted through channels
				 */
				using value_type = typename channel_type::value_type;

				/**
				 * A (safe) pointer to the actor
				 */
				actor_type* actor;

				/**
				 * The handle to the channel's lock
				 */
				std::unique_lock<lock_type> lock_;

				/**
				 * Construct a trait from a pointer to its actor
				 * @param act being a (safe) pointer to the actor
				 * @throws scl::exceptions::NonNullViolation
				 */
				explicit channel_actor_traits(actor_type* act) : actor{act} {
					if(!act)
						throw scl::exceptions::NonNullViolation{};

					this->lock_ = {this->actor->channel->lock, std::defer_lock};
				}

				/**
				 * Lock the channel
				 */
				void lock(){
					this->lock_.lock();
				}

				/**
				 * Unlock the channel
				 */
				void unlock(){
					this->lock_.unlock();
				}

				/**
				 * Notify the channel
				 */
				void notify(){
					this->actor->channel->condition.notify_one();
				}

				/**
				 * Wait for the channel
				 */
				void wait(){
					this->lock();
					this->actor->channel->condition.wait(this->lock_);
				}

				/**
				 * Wait for the channel until a condition is met
				 * @tparam Pred being the type of the predicate
				 * @param pred being the predicate to match
				 */
				template <class Pred>
				void waitUntil(Pred&& pred){
					this->lock();
					this->actor->channel->condition.wait(this->lock_, pred);
				}
			};
		}

		/**
		 * A channel to use for asynchronous communication of data in a queued manner
		 * @tparam T being the type of values
		 * @tparam Lock being the type of lock to use
		 * @tparam Guard being the type of guard to use
		 * @tparam Container being the container to use for the queue
		 */
		template <class T, class Lock = std::mutex, template<class> class Guard = std::lock_guard, class Container = std::deque<T>>
		class Channel{
			public:
				using lock_type = Lock;

				template <class L>
				using guard_type = Guard<L>;
				using queue_type = std::queue<T, Container>;
				using value_type = typename queue_type::value_type;

				using sender_type = details::ChannelSender<Channel>;
				using receiver_type = details::ChannelReceiver<Channel>;
				using transport_traits = details::channel_transport_traits<Channel>;
				using transport_type = typename transport_traits::type;

				friend sender_type;
				friend receiver_type;
				friend transport_type std::get(Channel&);

			protected:
				mutable lock_type lock;
				mutable std::condition_variable_any condition;
				queue_type queue;

				/**
				 * Implementation details to get a receiver and sender for this channel
				 * @return the transport payload
				 */
				transport_type interface(){
					return transport_traits::factory(sender_type{this}, receiver_type{this});
				}

			public:
				Channel() : lock{}, condition{}, queue{} {
				}

				Channel(const Channel&) = delete;
				Channel(Channel&&) = delete;
				Channel& operator=(const Channel&) = delete;
				Channel& operator=(Channel&&) = delete;
		};

		namespace details{
			/**
			 * Class used to send data to a Channel
			 * @tparam Chan being the channel type
			 */
			template <class Chan>
			class ChannelSender{
				public:
					using channel_type = Chan;
					using sender_traits = channel_actor_traits<ChannelSender>;
					using lock_type = typename sender_traits::lock_type;

					template <class L>
					using guard_type = typename sender_traits::guard_type<L>;
					using queue_type = typename sender_traits::queue_type;
					using value_type = typename sender_traits::value_type;

					friend sender_traits;

				protected:
					Chan* channel;
					sender_traits traits;

				public:
					/**
					 * Construct a sender from a (safe) pointer to its channel
					 */
					explicit ChannelSender(Chan* chan) : channel{chan}, traits{this} {
						if(!chan)
							throw scl::exceptions::NonNullViolation{};
					}

				protected:
					template <class... Args>
					ChannelSender& doPush(Args&&... args){
						guard_type<sender_traits> _{this->traits};
						this->channel->queue.emplace(std::forward<Args>(args)...);
						this->traits.notify();
						return *this;
					}

				public:
					ChannelSender& push(const value_type& value){
						return this->doPush(value);
					}

					ChannelSender& push(value_type&& value){
						return this->doPush(std::move(value));
					}

					template <class... Args>
					ChannelSender& pushEmplace(Args&&... args){
						return this->doPush(std::forward<Args&&>(args)...);
					}

					/**
					 * Alias for ChannelSender::push
					 */
					ChannelSender& queue(const value_type& value){ return this->push(value); }
					ChannelSender& queue(value_type&& value){ return this->push(std::move(value)); }
					ChannelSender& enqueue(const value_type& value){ return this->push(value); }
					ChannelSender& enqueue(value_type&& value){ return this->push(std::move(value)); }
					ChannelSender& send(const value_type& value){ return this->push(value); }
					ChannelSender& send(value_type&& value){ return this->queue(std::move(value)); }
			};

			template <class Chan>
			class ChannelReceiver{
				public:
					using channel_type = Chan;
					using receiver_traits = channel_actor_traits<ChannelReceiver>;
					using lock_type = typename receiver_traits::lock_type;

					template <class L>
					using guard_type = typename receiver_traits::guard_type<L>;
					using queue_type = typename receiver_traits::queue_type;
					using value_type = typename receiver_traits::value_type;
					using optional_type = scl::utils::Optional<value_type>;

					friend receiver_traits;

				protected:
					Chan* channel;
					receiver_traits traits;

				public:
					explicit ChannelReceiver(Chan* chan) : channel{chan}, traits{this} {
						if(!chan)
							throw scl::exceptions::NonNullViolation{};
					}

				public:
					value_type pop(){
						this->traits.waitUntil([&]{
							return !this->channel->queue.empty();
						});

						auto value = this->channel.queue.front();
						this->channel.queue.pop();
						this->traits.unlock(); //TODO: Check if necessary
						return value;
					}

					value_type dequeue(){
						return this->doPop();
					}

					value_type receive(){
						return this->doPop();
					}
			};
		}
	}
}

namespace std{
	template <class... Args>
	auto get(scl::async::Channel<Args...>& channel) -> typename decltype(channel)::transport_type{
		return channel.interface();
	}

	template <class... Args>
	struct tuple_size<scl::async::Channel<Args...>> : std::tuple_size<typename scl::async::Channel<Args...>::transport_type>{
	};
}