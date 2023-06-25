#pragma once

#include "Serializable.h"
#include "Serializer.h"

#include <type_traits>
#include <utility>
#include <vector>

template <typename ElemT, bool IsDynamic = false>
class Array : public Serializable
{
public:
    void Serialize(Serializer& aSerializer) override;
    int Count() const;
    void PushBack(const ElemT& anElement);

    // Adapter types and methods
    using Iterator = std::vector<ElemT>::iterator;
    using ConstIterator = std::vector<ElemT>::const_iterator;

    template <class... ValT>
    decltype(auto) EmplaceBack(ValT&&... someVals);
    Iterator begin() noexcept;
    ConstIterator begin() const noexcept;
    Iterator end() noexcept;
    ConstIterator end() const noexcept;
    ElemT& operator[](size_t idx);
    const ElemT& operator[](size_t idx) const;
    // ___

private:
    std::vector<ElemT> myElements;
};

template <typename ElemT>
using ArrayDynamic = Array<ElemT, true>;

template <typename ElemT, bool IsDynamic>
inline void Array<ElemT, IsDynamic>::Serialize(Serializer& aSerializer)
{
    int count {static_cast<int>(myElements.size())};
    aSerializer.Process("myCount", count);
    myElements.resize(count);

    for (int i = 0; i < count; ++i)
    {
        if constexpr (IsDynamic)
        {
            SerializableDynamic* ptr {Unwrap(myElements[i])};
            aSerializer.Process(std::to_string(i).data(), ptr);
            if (Unwrap(myElements[i]) != ptr)
            {
                myElements[i] = Wrap<ElemT>(ptr);
            }
        }
        else
        {
            aSerializer.Process(std::to_string(i).data(), myElements[i]);
        }
    }
}

template <typename ElemT, bool IsDynamic>
inline int Array<ElemT, IsDynamic>::Count() const
{
    return static_cast<int>(myElements.size());
}

template <typename ElemT, bool IsDynamic>
inline void Array<ElemT, IsDynamic>::PushBack(const ElemT& anElement)
{
    myElements.push_back(anElement);
}

template <typename ElemT, bool IsDynamic>
template <class... ValT>
decltype(auto) Array<ElemT, IsDynamic>::EmplaceBack(ValT&&... someVals)
{
    myElements.emplace_back(std::forward<ValT>(someVals)...);
}

template <typename ElemT, bool IsDynamic>
inline ElemT& Array<ElemT, IsDynamic>::operator[](std::size_t idx)
{
    return myElements[idx];
}

template <typename ElemT, bool IsDynamic>
inline const ElemT& Array<ElemT, IsDynamic>::operator[](std::size_t idx) const
{
    return myElements[idx];
}

template <typename ElemT, bool IsDynamic>
inline Array<ElemT, IsDynamic>::Iterator Array<ElemT, IsDynamic>::begin() noexcept
{
    return myElements.begin();
}

template <typename ElemT, bool IsDynamic>
inline Array<ElemT, IsDynamic>::ConstIterator Array<ElemT, IsDynamic>::begin() const noexcept
{
    return myElements.begin();
}

template <typename ElemT, bool IsDynamic>
inline Array<ElemT, IsDynamic>::Iterator Array<ElemT, IsDynamic>::end() noexcept
{
    return myElements.end();
}

template <typename ElemT, bool IsDynamic>
inline Array<ElemT, IsDynamic>::ConstIterator Array<ElemT, IsDynamic>::end() const noexcept
{
    return myElements.end();
}
