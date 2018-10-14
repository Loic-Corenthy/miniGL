//===============================================================================================//
/*!
 *  \file      VertexBoneData.hpp
 *  \author    Loïc Corenthy
 *  \version   1.0
 */
//===============================================================================================//

#pragma once

#include <array>

namespace miniGL
{
    /*!
     *  \brief Simple class to keep bone parameters
     *  \details Parameters: ID and weight for each bone
     */
    template <unsigned int SIZE>
    class VertexBoneData
    {
    public:
        /*!
         *  \brief Default constructor
         */
        VertexBoneData(void);

        /*!
         *  \brief Add bone parameter for a vertex
         *  @param pBoneID is the ID of the bone
         *  @param pWeight is the weight associated to the bone for a particular vertex
         */
        void addBoneData(unsigned int pBoneID, float pWeight);

    private:
        /*!
         *  \brief Reset the parameters
         */
        void reset(void);

    private:
        std::array<unsigned int, SIZE> mID;
        std::array<float, SIZE> mWeight;

    }; // class VertexBoneData

    template <unsigned int SIZE>
    void VertexBoneData<SIZE>::addBoneData(unsigned int pBoneID, float pWeight)
    {
        for (unsigned int i = 0; i < mID.size(); ++i)
        {
            if (mWeight[i] == 0.0f)
            {
                mID[i] = pBoneID;
                mWeight[i] = pWeight;
                return;
            }
        }
    }

    template <unsigned int SIZE>
    VertexBoneData<SIZE>::VertexBoneData(void)
    {
        reset();
    }

    template <unsigned int SIZE>
    void VertexBoneData<SIZE>::reset(void)
    {
        for (auto id : mID)
            id = 0;

        for ( auto w : mWeight)
            w = 0.0f;
    }

} // namespace miniGL
