#pragma once
#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <queue>
#include <tuple>
#include <utility>

#include "../macros.h"

namespace scl {
    namespace async {
        template <class T, class Lock, class Guard, class Container>
        class Channel;

        namespace details {
            template <class>
            class ChannelEmitter;

            template <class>
            class ChannelReceiver;

            /**
             * Traits for the implementation of the transport of information in a
             * scl::async::Channel
             * @tparam Chan being the channel type
             */
            template <class Chan>
            struct channel_transport_traits {
                /**
                 * @typedef channel_type
                 * The type of the channel
                 */
                using channel_type = Chan;

                /**
                 * @typdef emitter_type
                 * The type of the emitter
                 */
                using emitter_type = typename channel_type::emitter_type;

                /**
                 * @typedef receiver_type
                 * The type of the receiver
                 */
                using receiver_type = typename channel_type::receiver_type;

                /**
                 * @typedef type
                 * The type of transport payload
                 */
                using type = std::pair<emitter_type&, receiver_type&>;

                static type factory(emitter_type& emitter, receiver_type& receiver) {
                    return type{emitter, receiver};
                }
            };

            template <class Actor>
            struct channel_actor_traits {
                /**
                 * @typedef actor_type
                 * The type of the actor
                 */
                using actor_type = Actor;

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
                 * The type of guard used to protect the lock
                 */
                using guard_type = typename channel_type::guard_type;

                /**
                 * @typedef value_type
                 * The type of values transmitted through channels
                 */
                using value_type = typename channel_type::value_type;

                /**
                 * @typedef queue_type
                 * The type of the queue used in the channel
                 */
                using queue_type = typename channel_type::queue_type;

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
                 */
                explicit channel_actor_traits(actor_type& act)
                    : actor{&act}, lock_{act.channel->lock, std::defer_lock} {}

                /**
                 * Lock the channel
                 */
                void lock() { this->lock_.lock(); }

                /**
                 * Unlock the channel
                 */
                void unlock() { this->lock_.unlock(); }

                /**
                 * Notify the channel
                 */
                void notify() { this->actor->channel->condition.notify_one(); }

                /**
                 * Wait for the channel
                 *
                 * @warning locks, will need to unlock afterward
                 */
                void wait() {
                    this->lock();
                    this->actor->channel->condition.wait(this->lock_);
                }

                /**
                 * Wait for the channel until a condition is met
                 * @tparam Pred being the type of the predicate
                 * @param pred being the predicate to match
                 *
                 * @warning locks, will need to unlock afterward
                 */
                template <class Pred>
                void waitUntil(Pred&& pred) {
                    this->lock();
                    this->actor->channel->condition.wait(this->lock_, std::forward<Pred>(pred));
                }

                /**
                 * Wait for a specific duration for the channel
                 * @tparam Rep being the time representation type
                 * @tparam Period being the time period type
                 * @param time being the time to wait
                 * @return TIMEOUT if it waited for time, NO_TIMEOUT otherwise
                 *
                 * @warning locks, will need to unlock afterward
                 */
                template <class Rep, class Period>
                std::cv_status waitFor(const std::chrono::duration<Rep, Period>& time) {
                    this->lock();
                    return this->actor->channel->condition.wait_for(this->lock_, time);
                }

                /**
                 * Wait for a given period of time until the predicate is satisfied
                 * @tparam Rep being the time representation type
                 * @tparam Period being the time period type
                 * @param time being the time to wait
                 * @param pred being the predicate to satisfy
                 * @return TRUE if the predicate was satisfied, FALSE otherwise
                 *
                 * @warning locks, will need to unlock afterward
                 */
                template <class Rep, class Period, class Pred>
                bool waitFor(const std::chrono::duration<Rep, Period>& time, Pred&& pred) {
                    this->lock();
                    return this->actor->channel->condition.wait_for(this->lock_, time,
                                                                    std::forward<Pred>(pred));
                }
            };
        }  // namespace details

        template <class T, class Lock = std::mutex, class Guard = std::lock_guard<Lock>,
                  class Container = std::deque<T>>
        class Channel {
        public:
            /**
             * @typdef lock_type
             * The type of locks used in the channel
             */
            using lock_type = Lock;

            /**
             * @typedef guard_type
             * The type of lock guard used in the channel
             */
            using guard_type = Guard;

            /**
             * @typdef queue_type
             * The type of the channel's queue
             */
            using queue_type = std::queue<T, Container>;

            /**
             * @typedef value_type
             * The type of values exposed through the channel
             */
            using value_type = typename queue_type::value_type;

            /**
             * @typedef size_type
             * The type used to quantize the size of the queue
             */
            using size_type = typename queue_type::size_type;

            /**
             * @typedef emitter_type
             * The type of emitters for the channel
             */
            using emitter_type = details::ChannelEmitter<Channel>;

            /**
             * @typedef receiver_type
             * The type of receivers for the channel
             */
            using receiver_type = details::ChannelReceiver<Channel>;

            /**
             * @typedef transport_traits
             * The type of the transport traits for the channel
             */
            using transport_traits = details::channel_transport_traits<Channel>;

            /**
             * @typedef transport_type
             * The type of the transport payload
             */
            using transport_type = typename transport_traits::type;

            friend emitter_type;
            friend receiver_type;
            friend details::channel_actor_traits<emitter_type>;
            friend details::channel_actor_traits<receiver_type>;

            template <size_t I, class U, class L, template <class> class G, class C>
            friend auto std::get(Channel<U, L, G, C>& chan) ->
                typename std::tuple_element<I, typename Channel<U, L, G, C>::transport_type>::type;

        protected:
            /**
             * The lock being used for the channel's queue
             */
            mutable lock_type lock;

            /**
             * The condition used to wait for the channel's queue
             */
            mutable std::condition_variable_any condition;

            /**
             * The underlying queue used to store data
             */
            queue_type queue;

            /**
             * The emitter for this channel
             */
            emitter_type emitter_;

            /**
             * The receiver for this channel
             */
            receiver_type receiver_;

            /**
             * Implementation details to get a receiver and emitter for this channel
             * @return the transport payload
             */
            transport_type interface() {
                return transport_traits::factory(this->emitter(), this->receiver());
            }

        public:
            /**
             * Construct a channel
             */
            Channel() : lock{}, condition{}, queue{}, emitter_{*this}, receiver_{*this} {}

            Channel(const Channel&) = delete;
            Channel(Channel&&) = delete;
            Channel& operator=(const Channel&) = delete;
            Channel& operator=(Channel&&) = delete;

            /**
             * Retrieve the size of the queue
             * @return the size of the queue
             */
            size_type size() const {
                SCL_MAYBE_UNUSED guard_type _{lock};
                return this->queue.size();
            }

            /**
             * Alias for Channel::size
             */
            size_type length() const { return this->size(); }

            /**
             * Determine whether or not the channel is currently empty
             * @return TRUE if it is, FALSE otherwise
             */
            bool isEmpty() const { return this->size() <= 0; }

            /**
             * Get a receiver for this channel
             * @return the receiver
             */
            receiver_type& receiver() { return this->receiver_; }

            /**
             * Get an emitter for this channel
             * @return the emitter
             */
            emitter_type& emitter() { return this->emitter_; }

            /**
             * Forward a value into the channel
             * @tparam U being the type of data to forward
             * @param value being the value to forward
             * @return a reference to this Channel's ChannelEmitter
             */
            template <class U>
            emitter_type& operator<<(U&& value) {
                return this->emitter() << std::forward<U>(value);
            }

            /**
             * Extract a value from the channel
             * @tparam U being the type to store the data into
             * @param value being where to store the data
             * @return a reference to this Channel's ChannelReceiver
             */
            template <class U>
            receiver_type& operator>>(U& value) {
                return this->receiver() >> value;
            }
        };

        /**
         * Forward a value into a channel
         * @tparam U being the type of data to forward
         * @tparam T being the type of values in the channel
         * @tparam Lock being the type of locks in the channel
         * @tparam Guard being the type of lock guards in the channel
         * @tparam Container being the type of container used in the channel
         * @param value being the value to forward
         * @param channel being the channel to forward the value into
         */
        template <class U, class T, class Lock, class Guard, class Container>
        void operator>>(U&& value, Channel<T, Lock, Guard, Container>& channel) {
            channel << std::forward<U>(value);
        }

        /**
         * Extract a value from the channel
         * @tparam U being the type to store the data into
         * @tparam T being the type of values in the channel
         * @tparam Lock being the type of locks in the channel
         * @tparam Guard being the type of lock guards in the channel
         * @tparam Container being the type of container used in the channel
         * @param value being the place to store the extracted data
         * @param channel being the channel to extract data from
         */
        template <class U, class T, class Lock, class Guard, class Container>
        void operator<<(U& value, Channel<T, Lock, Guard, Container>& channel) {
            channel >> value;
        }

        namespace details {
            /**
             * Class used to send data to a Channel
             * @tparam Chan being the channel type
             */
            template <class Chan>
            class ChannelEmitter {
            public:
                /**
                 * @typedef channel_type
                 * The type of the channel
                 */
                using channel_type = Chan;

                /**
                 * @typedef emitter_traits
                 * The traits of the emitter
                 */
                using emitter_traits = channel_actor_traits<ChannelEmitter>;

                /**
                 * @typedef lock_type
                 * The type of lock used in the emitter
                 */
                using lock_type = typename emitter_traits::lock_type;

                /**
                 * @typedef guard_type
                 * The type of guards used in the emitter
                 */
                using guard_type = typename emitter_traits::guard_type;

                /**
                 * @typedef queue_type
                 * The type of queue used in the channel
                 */
                using queue_type = typename emitter_traits::queue_type;

                /**
                 * @typedef value_type
                 * The type of values used in the channel
                 */
                using value_type = typename emitter_traits::value_type;

                friend emitter_traits;
                friend channel_type;

            protected:
                /**
                 * A (safe) pointer to the channel
                 */
                channel_type* channel;

                /**
                 * The traits for the emitter
                 */
                emitter_traits traits;

                /**
                 * Construct a emitter from a (safe) pointer to its channel
                 * @param chan being the channel to construct from
                 */
                explicit ChannelEmitter(channel_type& chan) : channel{&chan}, traits{*this} {}

                /**
                 * Implementation details to push data onto the channel
                 * @tparam Args being the type of the arguments for the constructor
                 * @param args being the arguments for the constructor
                 * @return a reference to this ChannelEmitter
                 */
                template <class... Args>
                ChannelEmitter& doPush(Args&&... args) {
                    SCL_MAYBE_UNUSED guard_type _{this->traits};
                    this->channel->queue.emplace(std::forward<Args>(args)...);
                    this->traits.notify();
                    return *this;
                }

            public:
                ChannelEmitter(const ChannelEmitter&) = delete;
                ChannelEmitter& operator=(const ChannelEmitter&) = delete;
                ChannelEmitter(ChannelEmitter&&) = delete;
                ChannelEmitter& operator=(ChannelEmitter&&) = delete;

                /**
                 * Forward a value into the queue
                 * @tparam U being the type of data to forward
                 * @param value being the value to forward
                 * @return a reference to this ChannelEmitter
                 */
                template <class U>
                ChannelEmitter& push(U&& value) {
                    return this->doPush(std::forward<U>(value));
                }

                /**
                 * Construct and push in-place the value
                 * @tparam Args being the types of arguments for the constructor
                 * @param args being the arguments for the constructor
                 * @return a reference to this ChannelEmitter
                 */
                template <class... Args>
                ChannelEmitter& pushEmplace(Args&&... args) {
                    return this->doPush(std::forward<Args&&>(args)...);
                }

                /**
                 * Alias for ChannelEmitter::push
                 */
                template <class U>
                ChannelEmitter& queue(U&& value) {
                    return this->push(std::forward<U>(value));
                }

                /**
                 * Alias for ChannelEmitter::push
                 */
                template <class U>
                ChannelEmitter& enqueue(U&& value) {
                    return this->push(std::forward<U>(value));
                }

                /**
                 * Alias for ChannelEmitter::push
                 */
                template <class U>
                ChannelEmitter& emit(U&& value) {
                    return this->push(std::forward<U>(value));
                }

                /**
                 * Alias for ChannelEmitter::push
                 */
                template <class U>
                ChannelEmitter& operator<<(U&& value) {
                    return this->push(std::forward<U>(value));
                }
            };

            template <class Chan>
            class ChannelReceiver {
            public:
                /**
                 * @typedef channel_type
                 * The type of the channel
                 */
                using channel_type = Chan;

                /**
                 * @typedef receiver_traits
                 * The type of the traits for this receiver
                 */
                using receiver_traits = channel_actor_traits<ChannelReceiver>;

                /**
                 * @typedef lock_type
                 * The type of locks used by the traits
                 */
                using lock_type = typename receiver_traits::lock_type;

                /**
                 * @typedef guard_type
                 * @tparam L being the type of the lock
                 * The type of lock guards used by the traits
                 */
                using guard_type = typename receiver_traits::guard_type;

                /**
                 * @typedef queue_type
                 * The type of queue used by the channel
                 */
                using queue_type = typename receiver_traits::queue_type;

                /**
                 * @typedef value_type
                 * The type of values used by the channel
                 */
                using value_type = typename receiver_traits::value_type;

                /**
                 * @typedef optional_type
                 * The alternative type when there might not be a value
                 */
                using optional_type = scl::utils::Optional<value_type>;

                friend receiver_traits;
                friend channel_type;

            protected:
                /**
                 * A (safe) pointer to the channel
                 */
                channel_type* channel;

                /**
                 * The traits for this receiver
                 */
                receiver_traits traits;

                /**
                 * Construct a receiver from its channel
                 * @param chan being the channel to construct from
                 */
                explicit ChannelReceiver(channel_type& chan) : channel{&chan}, traits{*this} {}

            public:
                ChannelReceiver(const ChannelReceiver&) = delete;
                ChannelReceiver& operator=(const ChannelReceiver&) = delete;
                ChannelReceiver(ChannelReceiver&&) = delete;
                ChannelReceiver& operator=(ChannelReceiver&&) = delete;

                /**
                 * Get a value from the channel
                 * @return the first remaining value
                 */
                value_type pop() {
                    this->traits.waitUntil([&] { return !this->channel->queue.empty(); });

                    auto value = this->channel->queue.front();  // get front
                    this->channel->queue.pop();                 // remove from queue
                    this->traits.unlock();
                    return value;
                }

                /**
                 * Wait for a given duration before popping
                 * @tparam Rep being the time representation type
                 * @tparam Period being the time period type
                 * @param duration being the time to wait
                 * @return NONE if it could not get a value, a value otherwise
                 */
                template <class Rep, class Period>
                optional_type tryPop(const std::chrono::duration<Rep, Period>& duration) {
                    bool isEmpty = this->traits.waitFor(
                        duration, [&] { return !this->channel->queue.empty(); });

                    if (isEmpty) return optional_type{};

                    auto value = this->channel->queue.front();  // get front
                    this->channel->queue.pop();                 // remove from queue
                    this->traits.unlock();
                    return optional_type{std::move(value)};
                }

                /**
                 * Alias for ChannelReceiver::receive
                 */
                value_type dequeue() { return this->pop(); }

                /**
                 * Alias for ChannelReceiver::receive
                 */
                value_type receive() { return this->pop(); }

                /**
                 * Alias for ChannelReceiver::tryPop
                 */
                template <class Rep, class Period>
                optional_type tryDeque(const std::chrono::duration<Rep, Period>& duration) {
                    return this->tryPop(duration);
                }

                /**
                 * Alias for ChannelReceiver::tryPop
                 */
                template <class Rep, class Period>
                optional_type tryReceive(const std::chrono::duration<Rep, Period>& duration) {
                    return this->tryPop(duration);
                }

                /**
                 * Alias for ChannelReceiver::receive
                 * @tparam U being the type to store the data in
                 * @return a reference to this ChannelReceiver
                 */
                template <class U>
                ChannelReceiver& operator>>(U& value) {
                    value = std::move(this->receive());
                    return *this;
                }
            };
        }  // namespace details
    }      // namespace async
}  // namespace scl

namespace std {
    /**
     * Get either the emitter or the receiver from the channel
     * @tparam I being the index in the tuple (emitter, receiver)
     * @tparam T being the type of values used in the channel
     * @tparam Lock being the type of locks for the channel
     * @tparam Guard being the type of guards for the channel
     * @tparam Container being the type of containers for the queue of the channel
     * @param channel being the channel to extract data from
     * @return the emitter or the receiver
     */
    template <size_t I, class T, class Lock, class Guard, class Container>
    auto get(scl::async::Channel<T, Lock, Guard, Container>& channel) ->
        typename std::tuple_element<
            I, typename scl::async::Channel<T, Lock, Guard, Container>::transport_type>::type {
        return std::get<I>(channel.interface());
    }

    template <class T, class Lock, class Guard, class Container>
    struct tuple_size<scl::async::Channel<T, Lock, Guard, Container>>
        : std::tuple_size<typename scl::async::Channel<T, Lock, Guard, Container>::transport_type> {
    };
}  // namespace std