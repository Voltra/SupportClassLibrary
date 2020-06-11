#pragma once
#include <queue>
#include <deque>
#include <initializer_list>
#include <scl/async/with.h>
#include <scl/utils/Optional.h>

namespace scl{
	namespace async{
		template <class T, class Lock = std::mutex, template<class> class Guard = std::lock_guard, class Container = std::deque<T>>
		class AsyncQueue{
			public:
				using queue_type = std::queue<T, Container>;
				using size_type = typename queue_type::size_type;
				using value_type = typename queue_type::value_type;

				using guarded_type = Mutexed<queue_type, Lock, Guard>;
				using optional_type = scl::utils::Optional<value_type>;

			protected:
				guarded_type queue_;

			public:
				AsyncQueue() : queue_{} {
				}

				AsyncQueue(AsyncQueue&&) noexcept = default;
				AsyncQueue& operator=(AsyncQueue&&) noexcept = default;
				AsyncQueue(const AsyncQueue&) = delete;
				AsyncQueue& operator=(const AsyncQueue&) = delete;

				AsyncQueue(std::initializer_list<value_type> list) : AsyncQueue(){
					with(this->queue_, [&](queue_type& queue){
						for(auto&& elem : list)
							queue.emplace(std::move(elem));
					});
				}


				/**
				 * Determine the size of the queue
				 * @return the actual size of the queue
				 */
				size_type size() const{
					return with(this->queue_, [&](const queue_type& queue){
						return queue.size();
					});
				}

				/**
				 * Alias for AsyncQueue::size
				 */
				size_type length() const{
					return this->size();
				}

				/**
				 * Determine whether or not the queue is empty
				 * @return TRUE if it is, FALSE otherwise
				 */
				bool isEmpty() const{
					return this->size() <= 0;
				}

				/**
				 * Retrieve the front element of the queue
				 * @return NONE if there is none, the element otherwise
				 */
				optional_type front() const{ //TODO: Modify to use optional ref?
					return with(this->queue_, [&](const queue_type& queue){
						return queue.empty() ? optional_type{} : optional_type{queue.front()};
					});
				}

				/**
				 * Retrieve the back element of the queue
				 * @return NONE if there is none, the element otherwise
				 */
				optional_type back() const{
					return with(this->queue_, [&](const queue_type& queue){
						return queue.empty() ? optional_type{} : optional_type{queue.back()};
					});
				}

				/**
				 * Alias for AsyncQueue::front
				 */
				optional_type first() const{ return this->front(); }
				optional_type head() const{ return this->front(); }

				/**
				 * Alias for AsyncQueue::back
				 */
				optional_type last() const{ return this->back(); }
				optional_type tail() const{ return this->back(); }

				/**
				 * Add an element to the queue by copy
				 * @param elem being the element to copy into the queue
				 * @return a reference to this AsyncQueue
				 */
				AsyncQueue& push(const value_type& elem){
					return with(this->queue_, [&](queue_type& queue){
						queue.emplace(elem);
						return *this;
					});
				}

				/**
				 * Add an element to the queue by move
				 * @param elem being the element to move into the queue
				 * @return a reference to this AsyncQueue
				 */
				AsyncQueue& push(value_type&& elem){
					return with(this->queue_, [&](queue_type& queue){
						queue.emplace(std::move(elem));
						return *this;
					});
				}

				/**
				 * Construct and add an element to the queue
				 * @tparam Args being the types of the arguments for the constructor
				 * @param args being the arguments for the constructor
				 * @return a reference to this AsyncQueue
				 */
				template <class... Args>
				AsyncQueue& pushEmplace(Args&&... args){
					return with(this->queue_, [&](queue_type& queue){
						queue.emplace(std::forward<Args>(args)...);
						return this;
					});
				}

				/**
				 * Removes the first element of the queue
				 * @return NONE if there was none, the element otherwise
				 */
				optional_type pop(){
					return with(this->queue_, [](queue_type& queue){
						optional_type ret = queue.empty() ? optional_type{} : optional_type{queue.front()};
						queue.pop();
						return ret;
					});
				}


				/**
				 * Alias for AsyncQueue::push
				 */
				AsyncQueue& queue(const value_type& elem){ return this->push(elem); }
				AsyncQueue& enqueue(const value_type& elem){ return this->push(elem); }
				AsyncQueue& queue(value_type&& elem){ return this->push(std::move(elem)); }
				AsyncQueue& enqueue(value_type&& elem){ return this->push(std::move(elem)); }

				/**
				 * Alias for AsyncQueue::pop
				 */
				optional_type dequeue(){ return this->pop(); }
		};
	}
}