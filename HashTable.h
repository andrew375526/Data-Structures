#pragma once

#include <cmath>
#include <cstddef>
#include <iostream>
#include <optional>
#include <ranges>
#include <utility>
#include <vector>

template <typename T, typename U>
class HashTable;

template <typename T, typename U>
std::ostream& operator<<(std::ostream&, const HashTable<T, U>&);

template <typename T, typename U>
class HashTable final
{
public:
	HashTable() = default;
	HashTable(std::size_t capacity);
	bool insert(const T& key, const U& value);
	bool remove(const T& key);
	U* operator[](const T& key);
	void clear() noexcept;
	std::ptrdiff_t getLength() const;
	bool empty() const noexcept;
	friend std::ostream& operator<<<>(std::ostream&, const HashTable&);

private:
	std::vector<std::optional<std::pair<T, U>>> m_data{};
	std::ptrdiff_t hash(const T& key) const;
	std::optional<std::pair<T, U>> getPair();
};

template <typename T, typename U>
HashTable<T, U>::HashTable(std::size_t capacity)
	: m_data(capacity)
{
}

template <typename T, typename U>
bool HashTable<T, U>::insert(const T& key, const U& value)
{
	if (m_data.capacity() <= 0) return false;

	for (auto i{ hash(key) }; i < getLength(); ++i)
	{
		if (!std::data(m_data)[i])
		{
			std::data(m_data)[i] = std::pair{ key, value };
			return true;
		}

		if (std::data(m_data)[i]->first == key) return false;
	}

	for (auto i{ 0Z }; i < hash(key); ++i)
	{
		if (!std::data(m_data)[i])
		{
			std::data(m_data)[i] = std::pair{ key, value };
			return true;
		}

		if (std::data(m_data)[i]->first == key) return false;
	}

	return false;
}

template <typename T, typename U>
bool HashTable<T, U>::remove(const T& key)
{
	if (empty()) return false;
	if (getBucket(key).empty()) return false;

	for (auto i{ getBucket(key).cbegin() }; i != getBucket(key).cend(); ++i)
	{
		if (i->first != key) continue;
		getBucket(key).erase(i);
		return true;
	}

	return false;
}

template <typename T, typename U>
U* HashTable<T, U>::operator[](const T& key)
{
	for (auto i{ hash(key) }; i < getLength(); ++i)
	{
		std::optional pair{ std::data(m_data)[i] };

		if (pair && pair->first == key)
			return &(pair->second);
	}

	for (auto i{ 0Z }; i < hash(key); ++i)
	{
		std::optional pair{ std::data(m_data)[i] };

		if (pair && pair->first == key)
			return &(pair->second);
	}

	return nullptr;
}

template <typename T, typename U>
void HashTable<T, U>::clear() noexcept
{
	return m_data.clear();
}

template <typename T, typename U>
std::ptrdiff_t HashTable<T, U>::getLength() const
{
	return std::ssize(m_data);
}

template <typename T, typename U>
bool HashTable<T, U>::empty() const noexcept
{
	return std::empty(m_data);
}

template <typename T, typename U>
std::ostream& operator<<<>(std::ostream& out, const HashTable<T, U>& table)
{
	std::pair<T, U> lastPair{};

	for (const std::optional& pair : std::views::reverse(table.m_data))
	{
		if (!pair) continue;
		lastPair = *pair;
		break;
	}

	for (const std::optional& pair : table.m_data)
	{
		if (!pair) continue;
		out << pair->first << " " << pair->second;
		if (*pair != lastPair) out << "\n";
	}

	return out;
}

template <typename T, typename U>
std::ptrdiff_t HashTable<T, U>::hash(const T& key) const
{
	return std::abs(key % std::ssize(m_data));
}