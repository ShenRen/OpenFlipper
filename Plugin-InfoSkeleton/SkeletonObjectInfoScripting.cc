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
*   $Date$                    *
*                                                                            *
\*===========================================================================*/
#include "SkeletonObjectInfoPlugin.hh"
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

//-----------------------------------------------------------------------------
/** \brief set the descriptions for scripting slots
 *
 */
void InfoSkeletonObjectPlugin::setDescriptions()
{
	emit setSlotDescription("jointCount(int)",tr("get total number of joints for a given skeleton"),
			QStringList(tr("skeletontID")), QStringList(tr("id of a skeleton")));

	emit setSlotDescription("branchCount(int)",tr("get total number of branches for a given skeleton"),
			QStringList(tr("skeletonID")), QStringList(tr("id of a skeleton")));

	emit setSlotDescription("leafCount(int)",tr("get total number of leaves for a given skeleton"),
				QStringList(tr("skeletonID")), QStringList(tr("id of a skeleton")));



	emit setSlotDescription("boundingBoxMin(int)",tr("get minimum point of the axis-aligned bounding box"),
			QStringList(tr("skeletonID")), QStringList(tr("id of a skeleton")));

	emit setSlotDescription("boundingBoxMax(int)",tr("get maximum point of the axis-aligned bounding box"),
			QStringList(tr("skeletonID")), QStringList(tr("id of a skeleton")));

	emit setSlotDescription("boundingBoxSize(int)",tr("get the size of the axis-aligned bounding box"),
			QStringList(tr("skeletonID")), QStringList(tr("id of a skeleton")));


	emit setSlotDescription("boneLength(int,int)",tr("Get the length of a bone from given joint to his parent"),
			QString(tr("SkeletonID,jointID")).split(","),
			QString(tr("id of the skeleton, id of the joint")).split(","));

	emit setSlotDescription("minBoneLength(int)",tr("Get the minimal bone length of a skeleton"),
			QStringList(tr("SkeletonID")), QStringList(tr("id of the skeleton")));

	emit setSlotDescription("maxBoneLength(int)",tr("Get the maximal bone length of a skeleton"),
			QStringList(tr("SkeletonID")), QStringList(tr("id of the skeleton")));

	emit setSlotDescription("meanBoneLength(int)",tr("Get the mean bone length of a skeleton"),
			QStringList(tr("SkeletonID")), QStringList(tr("id of the skeleton")));
}

//-----------------------------------------------------------------------------

/** \brief get total number of vertices for a given object
 *
 * @param _skeletonID skeleton id
 * @return number of joints or -1 if an error occured
 */
int InfoSkeletonObjectPlugin::jointCount(int _skeletonID)
{
	Skeleton* skeleton = PluginFunctions::skeleton(_skeletonID);
	if ( !skeleton )
	{
		emit log(LOGERR, tr("Unable to get skeleton"));
		return -1;
	}

	return skeleton->jointCount();
}

//-----------------------------------------------------------------------------

/** \brief get total number of vertices for a given object
 *
 * @param _skeletonID skeleton id
 * @return number of branches (joints with multiple childs) or -1 if an error occured
 */
int InfoSkeletonObjectPlugin::branchCount(int _skeletonID)
{
	Skeleton* skeleton = PluginFunctions::skeleton(_skeletonID);
	if ( !skeleton )
	{
		emit log(LOGERR, tr("Unable to get skeleton"));
		return -1;
	}

	int result = 0;
	for (Skeleton::Iterator iter = skeleton->begin(); iter != skeleton->end(); ++iter)
		if (iter->size() > 1)
			++result;

	return result;
//safsdf
}

//-----------------------------------------------------------------------------

/** \brief get total number of vertices for a given object
 *
 * @param _skeletonID skeleton id
 * @return number of leaves or -1 if an error occured
 */
int InfoSkeletonObjectPlugin::leafCount(int _skeletonID)
{
	Skeleton* skeleton = PluginFunctions::skeleton(_skeletonID);
	if ( !skeleton )
	{
		emit log(LOGERR, tr("Unable to get skeleton"));
		return -1;
	}

	int result = 0;
	for (Skeleton::Iterator iter = skeleton->begin(); iter != skeleton->end(); ++iter)
		if (iter->size() == 0)
			++result;

	return result;
}

//-----------------------------------------------------------------------------

/** \brief get total number of vertices for a given object
 *
 * @param _skeletonID skeleton id
 * @return minimum point of the bounding box
 */
Vector InfoSkeletonObjectPlugin::boundingBoxMin(int _skeletonID)
{
	SkeletonObject* skeleton = PluginFunctions::skeletonObject(_skeletonID);
	if ( !skeleton )
	{
		emit log(LOGERR, tr("Unable to get skeleton"));
		return Vector();
	}

	Vector min,max;
	skeleton->skeletonNode()->boundingBox(min,max);
	return min;
}

//-----------------------------------------------------------------------------

/** \brief get total number of vertices for a given object
 *
 * @param _skeletonID skeleton id
 * @return maximum point of the bounding box
 */
Vector InfoSkeletonObjectPlugin::boundingBoxMax(int _skeletonID)
{
	SkeletonObject* skeleton = PluginFunctions::skeletonObject(_skeletonID);
	if ( !skeleton )
	{
		emit log(LOGERR, tr("Unable to get skeleton"));
		return Vector();
	}

	Vector min,max;
	skeleton->skeletonNode()->boundingBox(min,max);
	return max;
}

//-----------------------------------------------------------------------------

/** \brief get total number of vertices for a given object
 *
 * @param _skeletonID skeleton id
 * @return size of the bounding box
 */
Vector InfoSkeletonObjectPlugin::boundingBoxSize(int _skeletonID)
{
	SkeletonObject* skeleton = PluginFunctions::skeletonObject(_skeletonID);
	if ( !skeleton )
	{
		emit log(LOGERR, tr("Unable to get skeleton"));
		return Vector();
	}

	Vector min,max;
	skeleton->skeletonNode()->boundingBox(min,max);
	return (max - min);
}

//-----------------------------------------------------------------------------
/** \brief get the length of a bone from a given joint to his parent
 *
 * @param _skeletonID id of a skeleton
 * @param _jointID id of the joint
 * @return bone length (from _jointID to the parent of _jointID)
 */
double InfoSkeletonObjectPlugin::boneLength(int _skeletonID, int _jointID)
{
	Skeleton* skeleton = PluginFunctions::skeleton(_skeletonID);
	if ( !skeleton )
	{
		emit log(LOGERR, tr("Unable to get skeleton"));
	}

	Skeleton::Joint* parent = skeleton->joint(_jointID)->parent();
	if (!parent)
	{
		emit log(LOGERR, tr ("Unable to get parent joint"));
	}

	unsigned int parentID = parent->id();

	//length of bones is defined in the reference pose and cannot be modified in any animation
	Skeleton::Pose* pose = skeleton->referencePose();

	return (pose->globalTranslation(_jointID) - pose->globalTranslation(parentID)).length();
}

//-----------------------------------------------------------------------------

/** \brief get total number of vertices for a given object
 *
 * @param _skeletonID skeleton id
 * @return minimal bone length or -1 if an error occured
 */
double InfoSkeletonObjectPlugin::minBoneLength(int _skeletonID)
{
	Skeleton* skeleton = PluginFunctions::skeleton(_skeletonID);
	if ( !skeleton )
	{
		emit log(LOGERR, tr("Unable to get skeleton"));
		return -1;
	}


	double min = std::numeric_limits<double>::max();
	for (Skeleton::Iterator iter = skeleton->begin(); iter != skeleton->end(); ++iter)
	{
		if (!iter->isRoot())
		{
			double length = boneLength(_skeletonID,iter->id());
			if (length < min)
				min = length;
		}
	}
	return min;
}

//-----------------------------------------------------------------------------

/** \brief get total number of vertices for a given object
 *
 * @param _skeletonID skeleton id
 * @return maximal bone length or -1 if an error occured
 */
double InfoSkeletonObjectPlugin::maxBoneLength(int _skeletonID)
{
	Skeleton* skeleton = PluginFunctions::skeleton(_skeletonID);
	if ( !skeleton )
	{
		emit log(LOGERR, tr("Unable to get skeleton"));
		return -1;
	}


	double max = std::numeric_limits<double>::min();
	for (Skeleton::Iterator iter = skeleton->begin(); iter != skeleton->end(); ++iter)
	{
		if (!iter->isRoot())
		{
			double length = boneLength(_skeletonID,iter->id());
			if (length > max)
				max = length;
		}
	}
	return max;
}

//-----------------------------------------------------------------------------

/** \brief get total number of vertices for a given object
 *
 * @param _skeletonID skeleton id
 * @return mean bone length or -1 if an error occured
 */
double InfoSkeletonObjectPlugin::meanBoneLength(int _skeletonID)
{
	Skeleton* skeleton = PluginFunctions::skeleton(_skeletonID);
	if ( !skeleton )
	{
		emit log(LOGERR, tr("Unable to get skeleton"));
		return -1;
	}


	double sum = 0;
	unsigned int count = 0;
	for (Skeleton::Iterator iter = skeleton->begin(); iter != skeleton->end(); ++iter)
	{
		if (!iter->isRoot())
		{
			sum += boneLength(_skeletonID,iter->id());
			++count;
		}
	}

	return (sum/static_cast<double>(count));
}
