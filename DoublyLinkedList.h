#pragma once

#include <initializer_list>
#include <iostream>
#include <ranges>
#include <type_traits>
#include <utility>

template <typename T>
class DoublyLinkedList;

template <typename T>
std::ostream& operator<<(std::ostream&, const DoublyLinkedList<T>&);

template <typename T>
constexpr void swap(DoublyLinkedList<T>&, DoublyLinkedList<T>&) noexcept;

template <typename T>
class DoublyLinkedList final
{
public:
	constexpr DoublyLinkedList() = default;
	constexpr DoublyLinkedList(std::initializer_list<T> items);
	constexpr void insert(const T& item);
	constexpr void insert(T&& item);
	constexpr bool remove(const T& item) noexcept;
	constexpr const T* search(const T& item) const;
	constexpr T* search(const T& item);
	constexpr void reverse();
	friend std::ostream& operator<<<>(std::ostream&, const DoublyLinkedList&);

	friend constexpr void swap<>(DoublyLinkedList&
		, DoublyLinkedList&) noexcept;

	constexpr DoublyLinkedList(const DoublyLinkedList&);
	constexpr DoublyLinkedList& operator=(DoublyLinkedList) noexcept;
	constexpr DoublyLinkedList(DoublyLinkedList&&) noexcept;
	constexpr ~DoublyLinkedList();

private:
	struct Node
	{
		T item{};
		Node* prev{};
		Node* next{};
	};

	Node* m_head{ nullptr };
	Node* m_tail{ nullptr };
	bool m_reversed{ false };
	constexpr Node* searchNode(const T& item) const noexcept;
	constexpr void insert(Node*);
};

template <typename T>
constexpr DoublyLinkedList<T>::Node* DoublyLinkedList<T>::searchNode(
	const T& item) const noexcept
{
	auto* head{ m_head };

	while (head)
	{
		if (head->item == item) return head;
		head = head->next;
	}

	return nullptr;
}

template <typename T>
constexpr void DoublyLinkedList<T>::insert(Node* node)
{
	if (m_head) m_head->prev = node;
	else m_tail = node;
	m_head = node;
}

template <typename T>
constexpr DoublyLinkedList<T>::DoublyLinkedList(std::initializer_list<T> items)
	: DoublyLinkedList{}
{
	for (const auto& item : std::views::reverse(items)) insert(item);
}

template <typename T>
constexpr void DoublyLinkedList<T>::insert(const T& item)
{
	auto* node{ new Node{.item{item}, .prev{nullptr}, .next{m_head} } };
	insert(node);
}

template <typename T>
constexpr void DoublyLinkedList<T>::insert(T&& item)
{
	auto* node{ new Node{
		.item{std::move_if_noexcept(item)}, .prev{nullptr}, .next{m_head} } };

	insert(node);
}

template <typename T>
constexpr bool DoublyLinkedList<T>::remove(const T& item) noexcept
{
	const auto* itemNode{ searchNode(item) };
	if (!itemNode) return false;
	if (itemNode->next) itemNode->next->prev = itemNode->prev;
	else m_tail = itemNode->prev;
	if (itemNode->prev) itemNode->prev->next = itemNode->next;
	else m_head = itemNode->next;
	delete itemNode;
	return true;
}

template <typename T>
constexpr const T* DoublyLinkedList<T>::search(const T& item) const
{
	const auto* node{ searchNode(item) };
	return node ? &(node->item) : nullptr;
}

template <typename T>
constexpr T* DoublyLinkedList<T>::search(const T& item)
{
	return const_cast<T*>(std::as_const(*this).search(item));
}

template <typename T>
constexpr void DoublyLinkedList<T>::reverse()
{
	m_reversed = !m_reversed;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const DoublyLinkedList<T>& list)
{
	auto* start{ list.m_reversed ? list.m_tail : list.m_head };

	while (start)
	{
		out << start->item << " ";
		start = list.m_reversed ? start->prev : start->next;
	}

	return out;
}

template <typename T>
constexpr void swap(DoublyLinkedList<T>& list1
	, DoublyLinkedList<T>& list2) noexcept
{
	std::swap(list1.m_head, list2.m_head);
	std::swap(list1.m_tail, list2.m_tail);
}

template <typename T>
constexpr DoublyLinkedList<T>::DoublyLinkedList(
	const DoublyLinkedList& otherList)
{
	const auto* otherTail{ otherList.m_tail };

	while (otherTail)
	{
		insert(otherTail->item);
		otherTail = otherTail->prev;
	}
}

template <typename T>
constexpr DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(
	DoublyLinkedList otherList) noexcept
{
	swap(*this, otherList);
	return *this;
}

template <typename T>
constexpr DoublyLinkedList<T>::DoublyLinkedList(
	DoublyLinkedList&& otherList) noexcept
	: DoublyLinkedList{}
{
	swap(*this, otherList);
}

template <typename T>
constexpr DoublyLinkedList<T>::~DoublyLinkedList()
{
	while (m_head) remove(m_head->item);
}
