#pragma once
#include <atomic>
#include <functional>
#include <thread>

#include "./Channel.h"

namespace scl {
    namespace async {
        /**
         * A class that allow task execution on a separate thread using RAII semantics
         */
        class Active {
        public:
            /**
             * @typedef message_type
             * The type of messages sent through the queue
             */
            using message_type = std::function<void()>;

            /**
             * @typedef thread_type
             * The type of threads used to execute the tasks
             */
            using thread_type = std::thread;

            /**
             * @typedef flag_type
             * The type of flags used for completion checks
             */
            using flag_type = std::atomic_bool;

            /**
             * @typedef message_queue_type
             * The type of queue used to send messages
             */
            using message_queue_type = Channel<message_type>;

        protected:
            /**
             * The flag determining whether or not we should execute more tasks
             */
            flag_type done;

            /**
             * The queue used to transfer messages
             */
            message_queue_type mq;

            /**
             * The worker thread used to execute tasks
             */
            thread_type thread;

        public:
            /**
             * Construct an active object (starts spinning the queue)
             */
            Active()
                : done{false}, mq{}, thread{[this] {
                      auto&& receiver = mq.receiver();

                      while (!done) {
                          auto message = receiver.receive();
                          message();
                          std::this_thread::yield();
                      }
                  }} {}

            /**
             * Destroy an active object (finishing the queue)
             */
            ~Active() {
                this->execute([=] { done = true; });

                this->thread.join();
            }

            /**
             * Execute a task via the active object
             * @param msg being the task to execute
             */
            void execute(message_type msg) {
                if (done) return;

                this->mq.sender().send(msg);
            }
        };
    }  // namespace async
}  // namespace scl