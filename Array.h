#pragma once

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <utility>

template <typename T>
class Array;

template <typename T>
std::ostream& operator<<(std::ostream&, const Array<T>&);

template <typename T>
constexpr void swap(Array<T>&, Array<T>&) noexcept;

template <typename T>
class Array final
{
public:
	constexpr Array() = default;
	constexpr Array(std::initializer_list<T> items);
	constexpr Array(const Array&);
	constexpr Array(Array&&) noexcept;
	constexpr Array& operator=(Array) noexcept;
	constexpr const T& operator[](std::ptrdiff_t index) const;
	constexpr T& operator[](std::ptrdiff_t index);
	constexpr void insert(std::ptrdiff_t index, const T& value);
	constexpr void remove(std::ptrdiff_t index);
	constexpr void empty() noexcept;
	constexpr void resize(std::ptrdiff_t length);
	constexpr std::ptrdiff_t getLength() const;
	constexpr std::ptrdiff_t getCapacity() const;
	friend std::ostream& operator<<<>(std::ostream&, const Array&);
	friend constexpr void swap<>(Array&, Array&) noexcept;
	constexpr ~Array();

private:
	T* m_data{ nullptr };
	std::ptrdiff_t m_length{ 0Z };
	std::ptrdiff_t m_capacity{ 0Z };
};

template <typename T>
constexpr Array<T>::Array(std::initializer_list<T> items)
	: m_data{ new T[std::size(items)]{} }
	, m_length{ std::ssize(items) }
	, m_capacity{ std::ssize(items) }
{
	assert(std::ssize(items) >= 0 && "Length and capacity can't be negative.");
	for (auto i{ 0Z }; i < std::ssize(items); ++i)
		m_data[i] = std::data(items)[i];
}

template <typename T>
constexpr Array<T>::Array(const Array& a)
	: m_data{ new T[static_cast<std::size_t>(a.getLength())]{} }
	, m_length{ a.getLength() }
	, m_capacity{ a.getCapacity() }
{
	for (auto i{ 0Z }; i < a.getLength(); ++i)
		m_data[i] = a.m_data[i];
}

template <typename T>
constexpr Array<T>::Array(Array&& a) noexcept
	: Array{}
{
	swap(*this, a);
}

template <typename T>
constexpr Array<T>& Array<T>::operator=(Array a) noexcept
{
	swap(*this, a);
	return *this;
}

template <typename T>
constexpr const T& Array<T>::operator[](std::ptrdiff_t index) const
{
	assert(index >= 0 && index < getLength() && "Index out of bounds.");
	return m_data[index];
}

template <typename T>
constexpr T& Array<T>::operator[](std::ptrdiff_t index)
{
	return const_cast<T&>(std::as_const(*this)[index]);
}

template <typename T>
constexpr void Array<T>::insert(std::ptrdiff_t index, const T& value)
{
	assert(index >= 0 && index <= getLength() && "Index out of bounds.");
	resize(getLength() + 1);

	for (auto i{ getLength() - 1 }; i > index; --i)
		std::swap(m_data[i], m_data[i - 1]);

	m_data[index] = value;
}

template <typename T>
constexpr void Array<T>::remove(std::ptrdiff_t index)
{
	assert(index >= 0 && index < getLength() && "Index out of bounds.");

	for (auto i{ index }; i < getLength() - 1; ++i)
		std::swap(m_data[i], m_data[i + 1]);

	resize(getLength() - 1);
}

template <typename T>
constexpr void Array<T>::empty() noexcept
{
	delete[] m_data;
	m_data = nullptr;
	m_length = 0Z;
	m_capacity = 0Z;
}

template <typename T>
constexpr void Array<T>::resize(std::ptrdiff_t length)
{
	if (length <= getLength())
	{
		m_length = length;
		return;
	}

	auto* oldData{ m_data };
	m_data = new T[static_cast<std::size_t>(length)]{};

	for (auto i{ 0Z }; i < getLength(); ++i)
		m_data[i] = std::move_if_noexcept(oldData[i]);

	m_length = length;
	m_capacity = length;
	delete[] oldData;
}

template <typename T>
constexpr std::ptrdiff_t Array<T>::getLength() const
{
	return m_length;
}

template <typename T>
constexpr std::ptrdiff_t Array<T>::getCapacity() const
{
	return m_capacity;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Array<T>& a)
{
	for (auto i{ 0Z }; i < a.getLength(); ++i) out << a.m_data[i] << " ";
	return out;
}

template <typename T>
constexpr void swap(Array<T>& a1, Array<T>& a2) noexcept
{
	std::swap(a1.m_data, a2.m_data);
	std::swap(a1.m_length, a2.m_length);
	std::swap(a1.m_capacity, a2.m_capacity);
}

template <typename T>
constexpr Array<T>::~Array()
{
	delete[] m_data;
}
