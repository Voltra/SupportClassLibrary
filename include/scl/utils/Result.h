#pragma once
#include <scl/utils/Either.h>
#include <scl/utils/Placeholder.h>
#include <scl/exceptions/InvalidResultAccess.h>
#include <functional>

namespace scl{
	namespace utils{
		namespace details{
			/**
			 * //
			 */
			struct result_value final{};
			struct result_error final{};
		}

		template <class Value, class Error = Placeholder>
		class Result{
			public:
				using value_type = Value;
				using error_type = Error;
				using impl_type = Either<value_type, error_type>;

			protected:
				impl_type alt;

				Result() = delete;

				template <class T>
				Result(details::result_value, T&& value) : alt{impl_type::Left(std::forward<T>(value))} {
				}

				template <class E>
				Result(details::result_error, E&& err) : alt{impl_type::Right(std::forward<E>(err))} {
				}

			public:
				template <class T>
				static Result Ok(T&& value){
					return Result{
						details::result_value{},
						std::forward<T>(value)
					};
				}

				template <class T>
				static Result ok(T&& value){
					return Ok(std::forward<T>(value));
				}

				template <class... Args>
				static Result emplaceOk(Args&&... args){
					return Ok(value_type{
						std::forward<Args>(args)...
					});
				}

				template <class E>
				static Result Err(E&& err){
					return Result{
						details::result_error{},
						std::forward<E>(err)
					};
				}

				template <class E>
				static Result err(E&& error){
					return Err(std::forward<E>(error));
				}

				template <class... Args>
				static Result emplaceErr(Args&&... args){
					return Err(error_type{
						std::forward<Args>(args)...
					});
				}

			public:
				bool isOk() const{
					return this->alt.hasLeft();
				}

				bool isErr() const{
					return !this->isOk();
				}

				const value_type& value() const{
					try {
						return this->alt.getLeft();
					}catch(scl::exceptions::InvalidEitherAccess& e){
						throw scl::exceptions::InvalidResultAccess::okWhenErr();
					}
				}

				const value_type& get() const{
					return this->value();
				}

				const error_type& error() const{
					try{
						return this->alt.getRight();
					}catch(scl::exceptions::InvalidEitherAccess& e){
						throw scl::exceptions::InvalidResultAccess::errWhenOk();
					}
				}

				operator bool() const{
					return this->isOk();
				}

				operator const value_type&() const{
					return this->value();
				}

				const value_type& operator*() const{
					return this->get();
				}

				realConst(value_type*) operator->() const{
					return &(this->get());
				}

			public:
				Optional<value_type> toOptional() const{
					if(this->isOk())
						return this->value();
					return none;
				}

				Optional<value_type> asOptional() const{
					return this->toOptional();
				}

				template <class T = value_type>
				Result<T, error_type> map(std::function<T(const value_type&)> mapper) const{
					return this->isOk()
					? Result<T, error_type>::Ok(mapper(this->value()))
					: Result<T, error_type>::Err(this->error());
				}

				template <class T = value_type>
				Result<T, error_type> then(std::function<T(const value_type&)> mapper) const{
					return this->map(mapper);
				}

				template <class E = error_type>
				Result<value_type, E> mapError(std::function<E(const error_type&)> mapper) const{
					return this->isOk()
					? Result<value_type, E>::Ok(this->value())
					: Result<value_type, E>::Err(mapper(this->error()));
				}

				template <class T = value_type, class E = error_type>
				Result<T, E> mapBoth(std::function<T(const value_type&)> valueMapper, std::function<E(const error_type&)> errorMapper) const{
					//return this->map(valueMapper).mapError(errorMapper);
					return this->isOk()
					? Result<T, E>::Ok(valueMapper(this->value()))
					: Result<T, E>::Err(errorMapper(this->error()));
				}

				template <class T = value_type, class E = error_type>
				Result<T, E> flatMap(std::function<Result<T, E>(const value_type&)> mapper) const{
					return this->isOk()
					? mapper(this->value())
					: Result<T, E>::emplaceErr(this->error());
				}

				template <class T = value_type, class E = error_type>
				Result<T, E> andThen(std::function<Result<T, E>(const value_type&)> mapper) const{
					return this->flatMap(mapper);
				}

				template <class T = value_type, class E = error_type>
				Result<T, E> flatMapError(std::function<Result<T, E>(const error_type&)> mapper) const{
					return this->isOk()
					? Result<T, E>::emplaceOk(this->value())
					: mapper(this->error());
				}

			public:
				const value_type& okOr(const value_type& value) const{
					return this->isOk()
					? this->value()
					: value;
				}

				const value_type& okOr(std::function<const value_type&()> factory) const{
					return this->isOk()
					? this->value()
					: factory();
				}

				const error_type& errOr(const error_type& error) const{
					return this->isOk()
					? error
					: this->error();
				}

				const error_type& errOr(std::function<const error_type&()> factory) const{
					return this->isOk()
						   ? factory()
						   : this->error();
				}

				template <class E>
				const value_type& okOrThrow(E ex) const{
					if(!this->isOk())
						throw ex;
					return this->value();
				}
		};

		template <class T>
		Result<T, std::exception_ptr> wrapInResult(std::function<T()> f){
			try{
				auto value = f();
				return Result<T, std::exception_ptr>::emplaceOk(value);
			}catch(...){
				return Result<T, std::exception_ptr>::Err(
					std::current_exception()
				);
			}
		}
	}
}