#pragma once

class Bool{
	protected:
		bool value;

	public:
		Bool() = delete;
		explicit Bool(bool b) : value{b} {
		};
		Bool(const Bool&) = default;
		Bool(Bool&&) = default;

		Bool& operator=(const Bool&) = default;
		Bool& operator=(Bool&&) noexcept = default;

		operator bool() const{ return value; }

		template <class B>
		Bool implies(B b) const{ return Bool{!(*this) || b}; }

		template <class B>
		Bool equiv(B b) const{ return Bool{b == bool(*this)}; }
};