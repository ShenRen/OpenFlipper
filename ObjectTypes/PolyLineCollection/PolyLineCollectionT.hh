/*===========================================================================*\
*                                                                            *
*                              OpenFlipper                                   *
 *           Copyright (c) 2001-2015, RWTH-Aachen University                 *
 *           Department of Computer Graphics and Multimedia                  *
 *                          All rights reserved.                             *
 *                            www.openflipper.org                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * This file is part of OpenFlipper.                                         *
 *---------------------------------------------------------------------------*
 *                                                                           *
 * Redistribution and use in source and binary forms, with or without        *
 * modification, are permitted provided that the following conditions        *
 * are met:                                                                  *
 *                                                                           *
 * 1. Redistributions of source code must retain the above copyright notice, *
 *    this list of conditions and the following disclaimer.                  *
 *                                                                           *
 * 2. Redistributions in binary form must reproduce the above copyright      *
 *    notice, this list of conditions and the following disclaimer in the    *
 *    documentation and/or other materials provided with the distribution.   *
 *                                                                           *
 * 3. Neither the name of the copyright holder nor the names of its          *
 *    contributors may be used to endorse or promote products derived from   *
 *    this software without specific prior written permission.               *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED *
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           *
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  *
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       *
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        *
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      *
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              *
*                                                                            *
\*===========================================================================*/

#pragma once

#include <ObjectTypes/PolyLine/PolyLineT.hh>

#include <queue>

template <typename T>
class PolyLineCollectionT{
public:
    typedef typename ACG::PolyLineT<T> PolyLine;

    struct iterator{
        iterator(typename std::vector<PolyLine*>::iterator _begin, typename std::vector<PolyLine*>::iterator _end) :
            i_(0),
            it_(_begin),
            end_(_end)
        { }

        PolyLine*& operator* (){
            return *it_;
        }

        PolyLine* operator-> (){
            return *it_;
        }

        void operator++(){
            ++i_;
            ++it_;
        }

        int idx(){
            return i_;
        }

        operator bool() const{
            return it_ != end_;
        }

        int i_;
        typename std::vector<PolyLine*>::iterator it_;
        typename std::vector<PolyLine*>::iterator end_;
    };

    struct index_iterator{
        index_iterator(std::vector<PolyLine*>& _lines, typename std::vector<size_t>::iterator _begin, typename std::vector<size_t>::iterator _end) :
            lines_(_lines),
            it_(_begin),
            end_(_end)
        { }

        PolyLine*& operator* (){
            return lines_[*it_];
        }

        PolyLine* operator-> (){
            return lines_[*it_];
        }

        void operator++(){
            ++it_;
        }

        int idx(){
            return *it_;
        }

        operator bool() const{
            return it_ != end_;
        }

        std::vector<PolyLine*>& lines_;
        typename std::vector<size_t>::iterator it_;
        typename std::vector<size_t>::iterator end_;
    };

public:


    /** \brief Destructor
     *
     *  The destructor will call delete on all PolyLines in the PolyLine Collection!
     */
    virtual ~PolyLineCollectionT(){
        const size_t n = poly_lines_.size();
        for(size_t i = 0; i < n; ++i){
            delete poly_lines_[i];
        }
    }

    /** Create a new polyline in the collection
     *
     * @param _poly_line The new polyline to be added to the collection
     * @return           Index of the new polyline in the collection
     */
    size_t add_poly_line(PolyLine* _poly_line){

        ;
        if(empty_slots_.empty()){

            const size_t new_idx = poly_lines_.size();
            poly_lines_.push_back(_poly_line);
            visible_.push_back(new_idx);
            return new_idx;

        } else {

            const size_t new_idx = empty_slots_.front();
            empty_slots_.pop();
            poly_lines_[new_idx] = _poly_line;
            visible_.push_back(new_idx);
            return new_idx;

        }

    }

    /** \brief Create empty polyline and return id
     *
     * @return Index of new polyline in collection
     */
    int new_poly_line(){

        PolyLine* pl = new PolyLine();
        pl->request_vertex_selections();
        pl->request_edge_selections();
        pl->request_vertex_vhandles();
        pl->request_vertex_ehandles();
        pl->request_vertex_fhandles();
        pl->request_edge_scalars();

        return add_poly_line(pl);
    }

    /// Reserve space for additional _count polylines
    void reserve(size_t _count){
        poly_lines_.reserve(poly_lines_.size() + _count);
    }


     //===========================================================================
     /** @name Access functions
     * @{ */
     //===========================================================================
      inline size_t n_polylines(){return poly_lines_.size();}
      inline PolyLine* polyline(size_t _i){return poly_lines_[_i];}

      inline size_t n_visible_polylines(){return visible_.size();}
      inline PolyLine* visible_polyline(size_t _i){return poly_lines_[visible_[_i]];}
    /** @} */


    //===========================================================================
    /** @name Iterators
    * @{ */
    //===========================================================================
      inline iterator iter(){return iterator(poly_lines_.begin(), poly_lines_.end());}
      inline index_iterator visible_iter(){return index_iterator(poly_lines_, visible_.begin(), visible_.end());}
      inline index_iterator selected_iter(){return index_iterator(poly_lines_, selected_.begin(), selected_.end());}
    /** @} */

    /** \brief Set all polylines to visible
     *
     */
    void set_visible_all()
    {
        size_t n_lines = poly_lines_.size();
        visible_.resize(n_lines);
        for(size_t i = 0; i < n_lines; ++i){
            visible_[i] = i;
        }
    }

    /** \brief Set given polylines to visible
     *
     * @param _visible PolyLine indices to set visible
     */
    void set_visible(const std::vector<size_t>& _visible)
    {
        visible_ = _visible;
    }

    /** \brief Set given polylines to selected
     *
     * @param _selected PolyLine indices to set selected
     */
    void set_selected(const std::vector<size_t>& _selected)
    {
        selected_ = _selected;
    }

    /// unselect all polylines in collection
    void clear_selection(){
        selected_.clear();
    }

    /** \brief Clear Collection
     *
     * This will call delete on all polylines in the collection!
     */
    void clear(){
        for (size_t i = 0; i < poly_lines_.size(); ++i)
        {
            delete poly_lines_[i];
        }
        poly_lines_.clear();
        visible_.clear();

        while(!empty_slots_.empty()) empty_slots_.pop();
    }

    /** \brief Remove one polyline from the collection
     *
     * This will also call delete on the given polyline!
     *
     * @param _id Id of the polyline to be removed
     */
    void remove_polyline(int _id){
        if(_id < int(poly_lines_.size()) && poly_lines_[_id] != 0){
            delete poly_lines_[_id];
            poly_lines_[_id] = 0;
            empty_slots_.push(_id);
        }
    }

    //===========================================================================
    /** @name Coloring
    * @{ */
    //===========================================================================
      inline void set_color(size_t _edge_scalar, const ACG::Vec4uc& _color) {color_map_[_edge_scalar] = _color;}
      inline ACG::Vec4uc color(size_t _edge_scalar) {return color_map_[_edge_scalar];}
    /** @} */

protected:
    std::vector<PolyLine*> poly_lines_;
    std::vector<size_t> visible_;
    std::vector<size_t> selected_;

    std::queue<size_t> empty_slots_;

    // Color map for edge scalars
    std::map<size_t, ACG::Vec4uc> color_map_;
};
