#pragma once

#include "Serializable.h"
#include "Serializer.h"

#include <unordered_map>
#include <utility>

template <typename KeyT, typename ElemT>
class Map : public Serializable
{
public:
    using Iterator = std::unordered_map<KeyT, ElemT>::iterator;
    using ConstIterator = std::unordered_map<KeyT, ElemT>::const_iterator;
    using ValueType = std::pair<KeyT, ElemT>;

    void Serialize(Serializer& aSerializer) override;

    int Count() const;
    bool Contains(const KeyT& aKey) const;
    ElemT* Find(const KeyT& aKey);
    const ElemT* Find(const KeyT& aKey) const;
    ElemT* Insert(const KeyT& aKey, const ElemT& anElement);
    ElemT* Insert(const KeyT& aKey, ElemT&& anElement);
    template <typename... ValT>
    ElemT* Emplace(const KeyT& aKey, ValT&&... someVals);
    ElemT& operator[](const KeyT& aKey);
    void RemoveAll();
    void Swap(Map& anOther);

    Iterator begin() noexcept;
    ConstIterator begin() const noexcept;
    Iterator end() noexcept;
    ConstIterator end() const noexcept;

private:
    std::unordered_map<KeyT, ElemT> myElements;
};

template <typename KeyT, typename ElemT>
inline void Map<KeyT, ElemT>::Serialize(Serializer& aSerializer)
{
    int elementsToProcessCount {static_cast<int>(myElements.size())};
    aSerializer.Process("myCount", elementsToProcessCount);

    for (auto& [constKey, value] : myElements)
    {
        KeyT key {constKey};
        aSerializer.Process("K", key);
        aSerializer.Process("V", value);
    }

    const int existingElementCount {static_cast<int>(myElements.size())};
    for (int i = 0; i < elementsToProcessCount - existingElementCount; ++i)
    {
        std::pair<KeyT, ElemT> pair;
        aSerializer.Process("K", pair.first);
        aSerializer.Process("V", pair.second);
        myElements.insert(std::move(pair));
    }
}

template <typename KeyT, typename ElemT>
inline int Map<KeyT, ElemT>::Count() const
{
    return myElements.size();
}

template <typename KeyT, typename ElemT>
inline bool Map<KeyT, ElemT>::Contains(const KeyT& aKey) const
{
    return myElements.contains(aKey);
}

template <typename KeyT, typename ElemT>
inline ElemT* Map<KeyT, ElemT>::Find(const KeyT& aKey)
{
    Iterator it {myElements.find(aKey)};
    if (it != myElements.end())
    {
        return &(it->second);
    }
    return nullptr;
}

template <typename KeyT, typename ElemT>
inline const ElemT* Map<KeyT, ElemT>::Find(const KeyT& aKey) const
{
    ConstIterator it {myElements.find(aKey)};
    if (it != myElements.end())
    {
        return &(it->second);
    }
    return nullptr;
}

template <typename KeyT, typename ElemT>
inline ElemT* Map<KeyT, ElemT>::Insert(const KeyT& aKey, const ElemT& anElement)
{
    auto [it, succeeded] = myElements.insert(std::make_pair(aKey, anElement));
    if (succeeded)
    {
        return &(it->second);
    }
    return nullptr;
}

template <typename KeyT, typename ElemT>
inline ElemT* Map<KeyT, ElemT>::Insert(const KeyT& aKey, ElemT&& anElement)
{
    auto [it, succeeded] = myElements.insert(std::make_pair(aKey, std::move(anElement)));
    if (succeeded)
    {
        return &(it->second);
    }
    return nullptr;
}

template <typename KeyT, typename ElemT>
template <typename... ValT>
inline ElemT* Map<KeyT, ElemT>::Emplace(const KeyT& aKey, ValT&&... someVals)
{
    auto [it, succeeded] = myElements.try_emplace(aKey, std::forward<ValT>(someVals)...);
    if (succeeded)
    {
        return &(it->second);
    }
    return nullptr;
}

template <typename KeyT, typename ElemT>
inline ElemT& Map<KeyT, ElemT>::operator[](const KeyT& aKey)
{
    return myElements[aKey];
}

template <typename KeyT, typename ElemT>
inline void Map<KeyT, ElemT>::RemoveAll()
{
    myElements.clear();
}

template <typename KeyT, typename ElemT>
inline void Map<KeyT, ElemT>::Swap(Map& anOther)
{
    myElements.swap(anOther.myElements);
}

template <typename KeyT, typename ElemT>
inline Map<KeyT, ElemT>::Iterator Map<KeyT, ElemT>::begin() noexcept
{
    return myElements.begin();
}

template <typename KeyT, typename ElemT>
inline Map<KeyT, ElemT>::ConstIterator Map<KeyT, ElemT>::begin() const noexcept
{
    return myElements.begin();
}

template <typename KeyT, typename ElemT>
inline Map<KeyT, ElemT>::Iterator Map<KeyT, ElemT>::end() noexcept
{
    return myElements.end();
}

template <typename KeyT, typename ElemT>
inline Map<KeyT, ElemT>::ConstIterator Map<KeyT, ElemT>::end() const noexcept
{
    return myElements.end();
}
