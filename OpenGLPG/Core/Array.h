#pragma once

#include "Serializable.h"
#include "Serializer.h"

#include <vector>

template <typename ElemT>
class Array : public Serializable
{
public:
    void Serialize(Serializer& aSerializer) override;
    int Count() const;
    void PushBack(const ElemT& anElement);

    ElemT& operator[](size_t idx);
    const ElemT& operator[](size_t idx) const;

private:
    std::vector<ElemT> myElements;
};

template <typename ElemT>
inline void Array<ElemT>::Serialize(Serializer& aSerializer)
{
    int count {static_cast<int>(myElements.size())};
    aSerializer.Process("myCount", count);
    myElements.resize(count);

    for (int i = 0; i < count; ++i)
    {
        aSerializer.Process(std::to_string(i).data(), myElements[i]);
    }
}

template <typename ElemT>
inline int Array<ElemT>::Count() const
{
    return static_cast<int>(myElements.size());
}

template <typename ElemT>
inline void Array<ElemT>::PushBack(const ElemT& anElement)
{
    myElements.push_back(anElement);
}

template <typename ElemT>
inline ElemT& Array<ElemT>::operator[](std::size_t idx)
{
    return myElements[idx];
}

template <typename ElemT>
inline const ElemT& Array<ElemT>::operator[](std::size_t idx) const
{
    return myElements[idx];
}
