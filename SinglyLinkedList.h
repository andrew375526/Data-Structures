#pragma once

#include <initializer_list>
#include <iostream>
#include <ranges>
#include <type_traits>
#include <utility>

template <typename T>
class SinglyLinkedList;

template <typename T>
std::ostream& operator<<(std::ostream&, const SinglyLinkedList<T>&);

template <typename T>
constexpr void swap(SinglyLinkedList<T>&, SinglyLinkedList<T>&) noexcept;

template <typename T>
class SinglyLinkedList final
{
public:
	struct Node
	{
		T item{};
		Node* next{};
	};

	constexpr SinglyLinkedList() = default;
	constexpr SinglyLinkedList(std::initializer_list<T> items);
	constexpr void insert(const T& item);
	constexpr void insert(T&& item);
	constexpr bool remove(const T& item) noexcept;
	constexpr const T* search(const T& item) const;
	constexpr T* search(const T& item);
	constexpr bool reverse();
	friend std::ostream& operator<<<>(std::ostream&, const SinglyLinkedList&);

	friend constexpr void swap<>(SinglyLinkedList&
		, SinglyLinkedList&) noexcept;

	constexpr SinglyLinkedList(const SinglyLinkedList&);
	constexpr SinglyLinkedList& operator=(SinglyLinkedList) noexcept;
	constexpr SinglyLinkedList(SinglyLinkedList&&) noexcept;
	constexpr ~SinglyLinkedList();

private:
	Node* m_head{ nullptr };
	constexpr Node* predecessor(const T& item) noexcept;
};

template <typename T>
constexpr SinglyLinkedList<T>::Node* SinglyLinkedList<T>::predecessor(
	const T& item) noexcept
{
	auto* head{ m_head };

	while (head)
	{
		if (head->next && head->next->item == item) return head;
		head = head->next;
	}

	return nullptr;
}

template <typename T>
constexpr SinglyLinkedList<T>::SinglyLinkedList(std::initializer_list<T> items)
	: SinglyLinkedList{}
{
	for (const auto& item : std::views::reverse(items)) insert(item);
}

template <typename T>
constexpr void SinglyLinkedList<T>::insert(const T& item)
{
	m_head = new Node{ item, m_head };
}

template <typename T>
constexpr void SinglyLinkedList<T>::insert(T&& item)
{
	m_head = new Node{ std::move_if_noexcept(item), m_head };
}

template <typename T>
constexpr bool SinglyLinkedList<T>::remove(const T& item) noexcept
{
	if (!m_head) return false;

	if (m_head->item == item)
	{
		auto* next{ m_head->next };
		delete m_head;
		m_head = next;
		return true;
	}

	auto* pred{ predecessor(item) };
	if (!pred) return false;
	auto* itemNode{ pred->next };
	pred->next = itemNode->next;
	delete itemNode;
	return true;
}

template <typename T>
constexpr const T* SinglyLinkedList<T>::search(const T& item) const
{
	auto* head{ m_head };

	while (head)
	{
		if (head->item == item) return &(head->item);
		head = head->next;
	}

	return nullptr;
}

template <typename T>
constexpr T* SinglyLinkedList<T>::search(const T& item)
{
	return const_cast<T*>(std::as_const(*this).search(item));
}

template <typename T>
constexpr bool SinglyLinkedList<T>::reverse()
{
	if (!m_head) return false;
	auto* next{ m_head->next };
	m_head->next = nullptr;

	while (next)
	{
		auto* postnext{ next->next };
		next->next = m_head;
		m_head = next;
		next = postnext;
	}

	return true;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const SinglyLinkedList<T>& list)
{
	auto* head{ list.m_head };

	while (head)
	{
		out << head->item << " ";
		head = head->next;
	}

	return out;
}

template <typename T>
constexpr void swap(SinglyLinkedList<T>& list1
	, SinglyLinkedList<T>& list2) noexcept
{
	std::swap(list1.m_head, list2.m_head);
}

template <typename T>
constexpr SinglyLinkedList<T>::SinglyLinkedList(
	const SinglyLinkedList& otherList)
{
	const auto* otherHead{ otherList.m_head };

	while (otherHead)
	{
		insert(otherHead->item);
		otherHead = otherHead->next;
	}

	reverse();
}

template <typename T>
constexpr SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(
	SinglyLinkedList otherList) noexcept
{
	swap(*this, otherList);
	return *this;
}

template <typename T>
constexpr SinglyLinkedList<T>::SinglyLinkedList(
	SinglyLinkedList&& otherList) noexcept
	: SinglyLinkedList{}
{
	swap(*this, otherList);
}

template <typename T>
constexpr SinglyLinkedList<T>::~SinglyLinkedList()
{
	while (m_head) remove(m_head->item);
}
