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

/*===========================================================================*\
*                                                                            *
*   $Revision$                                                       *
*   $LastChangedBy$                                                *
*   $Date$                     *
*                                                                            *
\*===========================================================================*/


#ifndef RENDERERINFO_HH
#define RENDERERINFO_HH

#include <QString>
#include <QObject>
#include <ACG/Scenegraph/DrawModes.hh>
#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/PostProcessorInterface.hh>
#include <OpenFlipper/BasePlugin/RenderInterface.hh>
#include <OpenFlipper/common/GlobalDefines.hh>

#include <vector>

/** Type defining a currently loaded Renderer */
class DLLEXPORT RendererInfo{

  public :

    RendererInfo();

    RendererInfo(RenderInterface* _plugin,QString _name);

    /// Pointer to the loaded plugin (Already casted when loading it)
    RenderInterface*    plugin;

    /// Name of the plugin ( requested from the plugin on load)
    QString     name;

    /// Version of the plugin ( requested from the plugin on load)
    QString     version;

    /// Description of the plugin ( requested from the plugin on load)
    QString     description;

    /// Supported DrawModes
    ACG::SceneGraph::DrawModes::DrawMode modes;

    /// Possible action to add an options action or menu to the system.
    QAction* optionsAction;
};


class DLLEXPORT RenderManager {
  public:
    RenderManager();

    /** \brief Check if a renderer with the given name exists
     *
     * @param _name Name of the renderer
     * @return exists or not
     */
    bool rendererExists(QString _name);

    /**\brief  Get a new renderer Instance
     *
     * @param _name Name of the new renderer
     *
     * @return Pointer to renderer. If it exists, the existing one is returned!
     */
    RendererInfo* newRenderer(QString _name);

    /** \brief get renderer id with the given name
     *
     * @param _name Name of the renderer
     * @return Id of the renderer or -1 if it does not exist
     */
    int getRendererId(QString _name);

    /** \brief get renderer with the given name
     *
     * @param _name Name of the renderer
     * @return pointer or 0 if it does not exist
     */
    RendererInfo* getRenderer(QString _name);

    /** \brief count renderers for a DrawMode (excluding the default renderer)
     *
     * Checks in the list of renderers how many support the given draw mode combination.
     * The combination is checked as one complete block.
     *
     * @param _mode The mode that should be checked.
     *
     */
    int countRenderers(ACG::SceneGraph::DrawModes::DrawMode _mode);

    /** \brief Get the renderer with the given id
     *
     * @param _id Id of the renderer
     * @return
     */
    RendererInfo* operator[](unsigned int _id);

    /** \brief number of available renderers
     *
     * @return number of available renderers
     */
    size_t available();

    /** \brief set the active renderer
     *
     * @param _id viewer id
     * @param _active id of the renderer
     */
    void setActive(unsigned int _active, int _id);

    /** \brief set the active renderer
    *
    * @param _active name of the renderer
    * @param _id viewer id
    */
    void setActive(QString _active, int _id);

    /** \brief Get the current active renderer
     *
     * @param _id viewer id
     * @return Renderer
     */
    RendererInfo* active(int _id);

    /** \brief Get the id of the active renderer
     *
     * @param _id viewer id
     * @return renderer id
     */
    unsigned int activeId(int _id);

  private:
    /// Vector holding all available renderers
    std::vector<RendererInfo> availableRenderers_;

    /// The currently active renderer ids
    std::vector<unsigned int> activeRenderers_;
};

/// Get an instance of the render manager
DLLEXPORT
RenderManager& renderManager();

//===================================================================================
// Post processor Manager
//===================================================================================

/** Type defining a currently loaded Post processor */
class DLLEXPORT PostProcessorInfo{

  public :

    PostProcessorInfo();

    PostProcessorInfo(PostProcessorInterface* _plugin, QString _name);

    /// Pointer to the loaded plugin (Already casted when loading it)
    PostProcessorInterface*    plugin;

    /// Name of the plugin ( requested from the plugin on load)
    QString name;

    /// Description of the plugin
    QString description;

    /// Version of the plugin
    QString version;

    /// Possible action to add an options action or menu to the system.
    QAction* optionsAction;

};


class DLLEXPORT PostProcessorManager {
  public:

    PostProcessorManager();

    /** \brief Check if a post processor with the given name exists
     *
     * @param _name Name of the post processor
     * @return exists or not
     */
    bool postProcessorExists(QString _name);

    /**\brief  Get a new post processor Instance
     *
     * @param _name Name of the new post processor
     *
     * @return Pointer to post processor. If it exists, the existing one is returned!
     */
    PostProcessorInfo* newPostProcessor(QString _name);

    /** \brief get post processor with the given name
     *
     * @param _name Name of the post processor
     * @return pointer or 0 if it does not exist
     */
    PostProcessorInfo* getPostProcessor(QString _name);

    /** \brief Get the  post processor with the given id
    *
    * @param _id Id of the  post processor
    * @return
    */
    PostProcessorInfo* operator[](unsigned int _id);

    /** \brief number of available post processor
    *
    * @return number of available post processor
    */
    size_t available();

    /** \brief set the active post processor for viewer
    *
    * @param _viewerId viewer id
    * @param _active   id of the post processor
    */
    void setActive(unsigned int _active, int _viewerId);

    /** \brief Set the active post processor for viewer
    *
    * This will reset the current post processor chain to one
    * active post processor (the given one)
    *
    * @param _id viewer id
    * @param _active name of the post processor
    */
    void setActive(QString _active, int _id);


    //===========================================================================
    /** @name Support for multiple post-processors per viewer
     * @{ */
    //===========================================================================

    /** \brief Append the active post processor to the chain for viewer
    *
    * @param _viewerId viewer id
    * @param _active   id of the post processor
    */
    void append(unsigned int _active, int _viewerId);

    /** \brief Append the active post processor to the chain for viewer
    *
    * @param _id viewer id
    * @param _active name of the post processor
    */
    void append(QString _active, int _id);

    /** \brief Insert the active post processor to the chain for viewer
    *
    * @param _viewerId viewer id
    * @param _chainIdx Post processor chain index
    * @param _active   id of the post processor
    */
    void insert(unsigned int _active, int _chainIdx, int _viewerId);

    /** \brief Insert the active post processor to the chain for viewer
    *
    * @param _active name of the post processor
    * @param _chainIdx Post processor chain index
    * @param _id viewer id
    */
    void insert(QString _active, int _chainIdx, int _id);

    /** \brief Remove a post processor at the specified chain index
    *
    * @param _id viewer id
    * @param _chainIdx Post processor chain index
    */
    void remove(int _id, int _chainIdx);

    /** \brief Get the number of active post processors for viewer
    *
    * @param _id ViewerId
    * @return PostProcessor count
    */
    int numActive( int _id);

    /** @} */




    /** \brief Get the id of the active post processor for viewer at chain index
    *
    * @param _id ViewerId
    * @param _chainIdx Post processor chain index
    * @return post processor id
    */
    unsigned int activeId( int _id, int _chainIdx = 0);

    /** \brief Get the current active post processor for viewer at chain index
    *
    * @param _id ViewerId
    * @param _chainIdx Post processor chain index
    * @return post processor
    */
    PostProcessorInfo* active( int _id, int _chainIdx = 0);


  private:
    /// Vector holding all available  post processors
    std::vector<PostProcessorInfo> availablePostProcessors_;

    /// The currently active post processor chain
    std::vector<std::vector<unsigned int> > activePostProcessors_;
};

/// Get an instance of the Post Processor manager
DLLEXPORT
PostProcessorManager& postProcessorManager();

#endif //RENDERERINFO_HH
