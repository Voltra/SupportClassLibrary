[TOC]

# Work In Progress

# Presentation

The Support Class Library (or SCL, formerly named Standard Class Library as a nod to the STL) is a library built on top of the STL and wants to provide tools and interfaces for common problems and patterns in order to make application development easier.



# Parts

## Stream

The stream API is a library that allows lazy evaluation of potentially infinite sequences of data in an easy to write and easy to extend fashion. 



### Comparison with pure STL

For instance, a recurrent task is to filter through a collection and execute an action on the filtered elements, let's compare how one would approach this task in both pure STL and using `scl::stream`.

We will assume that the data is initially stored in a `std::vector`.



#### Pure STL solution

```
template <class Predicate, class Action>
void processActionIf(Predicate predicate, Action action){
	std::vector<std::shared_ptr<GuiComponent>> components = GUI.getComponents();
	for(GuiComponent& component : components){
		if(predicate(component))
			action(component);
	}
}

// OR

template <class Predicate, class Action>
void processActionIf(Predicate predicate, Action action){
	std::vector<std::shared_ptr<GuiComponent>> components = GUI.getComponents();
	std::vector<std::shared_ptr<GuiComponent>> filtered;
	
	std::copy_if(begin(components), end(components), emplace_back(filtered), predicate);
	std::for_each(begin(filtered), end(filtered), action);
}
```



In this simple use-case, using a for-loop reduces the amount of unnecessary copies that the otherwise functional-ish code would make. But things can get quite messy with a for-loop : conditions in conditions, chains of conditions, etc... . With the stream API, we want to get the best of both world : expressive and as efficient as a single for-loop.



#### Stream API

```
template <class Predicate, class Action>
void processActionIf(Predicate predicate, Action action){	
	streamFrom(GUI.getComponents())
	| filter(predicate)
	| forEach(action);
}
```

Here we get a lot of advantages :

* Because it has been developped with knowledge of what we encounter in C++, we do not have the "begin&end iterators" issues : we simply pass a container and use it as the data source
* We gain in expressiveness : `filter` expresses more intent than `copy_if` (because `copy_if` was not the actual intent, we used it to filter and use the result afterward)
* We gain in clarity : Loops can get messy, we can mismatch types. Here no type is specified for the components container nor the components themselves (this is true even when there's a lot to process, where loops could get messier)
* We gain in readability : `streamFrom` is where we begin the sequence processing, `|` indicate the start of a new operation, `;` ends the sequence processing



### Design

The library is greatly inspired by rangev3, Rust's `iter`, JS functional methods for arrays, the JS library `sequency` and Java 8's Stream API.



#### Why free functions and operators instead of methods?

In addition to the looks of it, using free functions and operators instead of methods provide one really important selling point : extendability.



With methods, if you want to add an operation for streams you would have to :

* Create a class that inherits from `scl::stream::Stream`
* Add your methods
* Tell the people that want to use your operation to use your class instead of `scl::stream::Stream`

One could argue that with some runtime polymorphism shenanigans provided by the base class we could achieve something similar but it would be painful for the SCL maintainers and probably for the library maintainers.



Using free functions, to add an operation, all you have to do is :

* Create an appropriate stream iterator class (if needed)
* Create a toolbox/type tag (if needed)
* Create the free function
* Create the `operator|` overload for your toolbox/type tag (if needed)

The pain is where it should be, on the library maintainers' side. Honestly, pain is a bit of an exageration.



You can have a look at `filter`/`map` to see how an operator is built from the ground up.

You can have a look at `unique` to see how an operator is built on top of another one.



#### What is the cost?

Let's say you have

```
std::vector<SpecialElement> v = streamFrom(container)
| map(&Element::getSpecialId)
| unique()
| filter(startsWith("s", Case::INSENSITIVE))
| map(+[](const SpecialId& id){ return SpecialHub::from(id, Checks::NONE); })
| pack::toVector();
```





This is roughly equivalent to

```
std::vector<SpecialElement> v;
std::set<SpecialId> tagged;

auto superSpecial = startsWith("s", Case::INSENSITIVE);
for(auto&& e : container){
    SpecialId id = e.getSpecialId();
    if(tagged.count(id))
        continue;
    else
        tagged.insert(id);
    
    if(!superSpecial(id))
        continue;
    
    v.emplace_back(SpecialHub::from(id, Checks::NONE));
}
```





"roughly" because we need to take in account the iterators lifetime as well as the space and state they may use.

Because they are all objects, the state is limited to the lifetime of the iterators/streams and cannot be accessed outside unlike `tagged` and `superSpecial`.



## Async API

The async API is a set of tools that abstract away all the burden of maintaining and using asynchrony.



### Guarded data and `Mutexed`

`scl::async::Mutexed<T>` solves one problem : automatic guard when using data across asynchronous contexts.

Used with `scl::async::with`, an abstraction over resource management (e.g. you could adapt it for the filesystem), it becomes one of the most useful tools to use the Monitor pattern :

```c++
#include "Queue.h"
#include <scl/async/async.hpp>
#include <scl/utils/utils.hpp>

using namespace scl::async;
using namespace scl::utils;

template <class T>
class AsyncQueue{
    protected:
    	Mutexed<Queue<T>> queue = {};
    
    public:
    	AsyncQueue(){}
    
    	AsyncQueue& push(T elem){ //endpoint
            with(this->queue, [](Queue<T>& queue){ //mutually exclusive
                queue.push(elem);
            });
            
            return *this;
        }
    
    	Optional<T> pop(){ //endpoint
            Optional<T> ret = none;
            
            with(this->queue, [&](Queue<T>& queue){ //mutually exclusive
                if(!queue.isEmpty())
                    ret = queue.pop();
            });
            
            return ret;
        }
};
```



### `with` clause

Think of `scl::async::with` as the `try-with-resources` of Java or the `using` of C# or even the classic RAII of C++, but with functions.



Why the use of functions instead of wrapping in a block? It's nicer to read and reason about :

* you need to remember to put the block of code around the exact portion of code that should be mutually exclusive
* you need to have a ready to go RAII lock or a way to emulate it (e.g. `lock` and `unlock`)
* the syntax is quite ugly



With the function approach, we're closer to monitors and actually have a distinctive way to describe operations and their limited scope. You often have a function (or a way to make one) when you're dealing with this kind of code, why not make one then?