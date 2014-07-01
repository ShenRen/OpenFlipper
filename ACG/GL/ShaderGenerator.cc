/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
 *      Copyright (C) 2001-2014 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openflipper.org                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 *  This file is part of OpenFlipper.                                        *
 *                                                                           *
 *  OpenFlipper is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as           *
 *  published by the Free Software Foundation, either version 3 of           *
 *  the License, or (at your option) any later version with the              *
 *  following exceptions:                                                    *
 *                                                                           *
 *  If other files instantiate templates or use macros                       *
 *  or inline functions from this file, or you compile this file and         *
 *  link it with other files to produce an executable, this file does        *
 *  not by itself cause the resulting executable to be covered by the        *
 *  GNU Lesser General Public License. This exception does not however       *
 *  invalidate any other reasons why the executable file might be            *
 *  covered by the GNU Lesser General Public License.                        *
 *                                                                           *
 *  OpenFlipper is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU LesserGeneral Public          *
 *  License along with OpenFlipper. If not,                                  *
 *  see <http://www.gnu.org/licenses/>.                                      *
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                       *
 *   $Author$                                                      *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/

#include "ShaderGenerator.hh"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>

namespace ACG
{

#define LIGHTING_CODE_FILE "ShaderGen/SG_LIGHTING.GLSL"

// attribute request keywords
#define SG_REQUEST_POSVS "#define SG_REQUEST_POSVS"
#define SG_REQUEST_POSOS "#define SG_REQUEST_POSOS"
#define SG_REQUEST_TEXCOORD "#define SG_REQUEST_TEXCOORD"
#define SG_REQUEST_VERTEXCOLOR "#define SG_REQUEST_VERTEXCOLOR"
#define SG_REQUEST_NORMAL "#define SG_REQUEST_NORMAL"


int ShaderProgGenerator::numModifiers_ = 0;
ShaderModifier* ShaderProgGenerator::modifiers_[32] = {0};



ShaderGenerator::ShaderGenerator()
  : version_(150), inputArrays_(false), outputArrays_(false)
{
}

ShaderGenerator::~ShaderGenerator()
{

}


void ShaderGenerator::initVertexShaderIO(const ShaderGenDesc* _desc, 
  bool _requestPosVS,
  bool _requestPosOS,
  bool _requestNormal,
  bool _requestTexCoord,
  bool _requestColor)
{
  // set type of IO
  inputArrays_ = false;
  outputArrays_ = false;
  inputPrefix_ = "in";         // inputs: inPosition, inTexCoord...
  outputPrefix_ = "outVertex"; // outputs: outVertexPosition, outVertexTexCoord..

  addInput("vec4 inPosition");
  addOutput("vec4 outVertexPosCS");


  if (_desc->shadeMode != SG_SHADE_UNLIT)
    addInput("vec3 inNormal");

  if (_desc->textured())
  {

    std::map<size_t,ShaderGenDesc::TextureType>::const_iterator iter = _desc->textureTypes().begin();

    /// TODO Setup for multiple texture coordinates as input
    if (iter->second.type == GL_TEXTURE_3D) {
      addInput("vec3 inTexCoord");
      addOutput("vec3 outVertexTexCoord");
    } else {
      addInput("vec2 inTexCoord");
      addOutput("vec2 outVertexTexCoord");
    }

  }


  if (_desc->vertexColors || _requestColor)
    addInput("vec4 inColor");


  if (_desc->shadeMode == SG_SHADE_PHONG)
  {
    addOutput("vec3 outVertexNormal");
    addOutput("vec4 outVertexPosVS");
  }


  std::string strColorOut = "";


  if (_desc->shadeMode == SG_SHADE_FLAT)
    if (!_desc->geometryTemplateFile.isEmpty())
      strColorOut = "vec4 outVertexColor";
    else {
      // Bypass the output setter, as we have to set that directly with the flat.
      addStringToList("vec4 outVertexColor", &outputs_, "flat out ", ";");
    }
  else {
    if (_desc->shadeMode == SG_SHADE_GOURAUD || _desc->vertexColors || _requestColor)
      strColorOut = "vec4 outVertexColor";
  }

  if (strColorOut.size())
    addOutput(strColorOut.c_str());


  // handle other requests: normals, positions, texcoords

  if (_requestPosVS)
    addOutput("vec4 outVertexPosVS");

  if (_requestPosOS)
    addOutput("vec4 outVertexPosOS");

  if (_requestTexCoord && !_desc->textured())
  {
    // assume 2d texcoords
    addInput("vec2 inTexCoord");
    addOutput("vec2 outVertexTexCoord");
  }

  if (_requestNormal)
  {
    addInput("vec3 inNormal");
    addOutput("vec3 outVertexNormal");
  }
}

void ShaderGenerator::initTessControlShaderIO(const ShaderGenDesc* _desc, ShaderGenerator* _prevStage) 
{
  // set type of IO
  inputArrays_ = true;
  outputArrays_ = true;
  inputPrefix_ = _prevStage->outputPrefix_;
  outputPrefix_ = "outTc"; // outputs: outTcPosition, outTcTexCoord..

  matchInputs(_prevStage, true, inputPrefix_, outputPrefix_);
}

void ShaderGenerator::initTessEvalShaderIO(const ShaderGenDesc* _desc, ShaderGenerator* _prevStage) 
{
  // set type of IO
  inputArrays_ = true;
  outputArrays_ = false;
  inputPrefix_ = _prevStage->outputPrefix_;
  outputPrefix_ = "outTe"; // outputs: outTePosition, outTeTexCoord..

  matchInputs(_prevStage, true, inputPrefix_, outputPrefix_);
}

void ShaderGenerator::initGeometryShaderIO(const ShaderGenDesc* _desc, ShaderGenerator* _prevStage) 
{
  // set type of IO
  inputArrays_ = true;
  outputArrays_ = false;
  inputPrefix_ = _prevStage->outputPrefix_;
  outputPrefix_ = "outGeometry"; // outputs: outGeometryPosition, outGeometryTexCoord..

  matchInputs(_prevStage, true, inputPrefix_, outputPrefix_);

  return;

  addInput("vec4 outVertexPosCS[]");
  addOutput("vec4 outGeometryPosCS");

  if (_desc->textured()) {

    std::map<size_t,ShaderGenDesc::TextureType>::const_iterator iter = _desc->textureTypes().begin();

    /// TODO Setup for multiple texture coordinates as input
    if (iter->second.type == GL_TEXTURE_3D)
    {
      addInput("vec3 outVertexTexCoord[]");
      addOutput("vec3 outGeometryTexCoord");
    } else {
      addInput("vec2 outVertexTexCoord[]");
      addOutput("vec2 outGeometryTexCoord");
    }

  }


  if (_desc->shadeMode == SG_SHADE_PHONG)
  {
    addInput("vec3 outVertexNormal[]");
    addInput("vec4 outVertexPosVS[]");

    addOutput("vec3 outGeometryNormal");
    addOutput("vec4 outGeometryPosVS");
  }

  QString strColorOut = "";

  if (_desc->shadeMode == SG_SHADE_FLAT || _desc->shadeMode == SG_SHADE_GOURAUD || _desc->vertexColors) {
    addInput("vec4 outVertexColor[]");


    if (_desc->shadeMode == SG_SHADE_FLAT)
      addStringToList("vec4 outGeometryColor", &outputs_, "flat out ", ";");
    else {
      if (_desc->shadeMode == SG_SHADE_GOURAUD || _desc->vertexColors)
        strColorOut = "vec4 outGeometryColor";
    }

    if ( !strColorOut.isEmpty() )
      addOutput(strColorOut);
  }

}



void ShaderGenerator::initFragmentShaderIO(const ShaderGenDesc* _desc, ShaderGenerator* _prevStage)
{
  // set type of IO
  inputArrays_ = false;
  outputArrays_ = false;
  inputPrefix_ = _prevStage->outputPrefix_;
  outputPrefix_ = "outFragment";

  matchInputs(_prevStage, false);
  addOutput("vec4 outFragment");
}




void ShaderGenerator::initDefaultUniforms()
{
  addUniform("mat4 g_mWVP" , "  // Projection * Modelview");       // Transforms directly from Object space to NDC
  addUniform("mat4 g_mWV" , "   // Modelview matrix");             // Modelview transforms from Object to World to View coordinates
  addUniform("mat3 g_mWVIT" , " // Modelview inverse transposed"); // Modelview inverse transposed, transforms from view across World into Object coordinates
  addUniform("mat4 g_mP", "     // Projection matrix");            // Projection Matrix

  addUniform("vec3 g_vCamPos");
  addUniform("vec3 g_vCamDir");

  addUniform("vec3 g_cDiffuse");
  addUniform("vec3 g_cAmbient");
  addUniform("vec3 g_cEmissive");
  addUniform("vec3 g_cSpecular");
  addUniform("vec4 g_vMaterial");
}


#define ADDLIGHT(x) (sz.sprintf(x"_%d", lightIndex_), addUniform(sz))

void ShaderGenerator::addLight(int lightIndex_, ShaderGenLightType _light)
{
  QString sz;

  ADDLIGHT("vec3 g_cLightDiffuse");
  ADDLIGHT("vec3 g_cLightAmbient");
  ADDLIGHT("vec3 g_cLightSpecular");

  if (_light == SG_LIGHT_POINT ||
    _light == SG_LIGHT_SPOT)
  {
    ADDLIGHT("vec3 g_vLightPos");
    ADDLIGHT("vec3 g_vLightAtten");
  }

  if (_light == SG_LIGHT_DIRECTIONAL ||
    _light == SG_LIGHT_SPOT)
    ADDLIGHT("vec3 g_vLightDir");


  if (_light == SG_LIGHT_SPOT)
    ADDLIGHT("vec2 g_vLightAngleExp");
}



void ShaderGenerator::addStringToList(QString _str, 
                                      QStringList* _arr,
                                      QString _prefix,
                                      QString _postfix)
{
  // Construct the whole string
  QString tmp = _prefix + _str + _postfix;

  // normalize string
  //  remove tabs, double whitespace
  tmp = tmp.simplified();

  // avoid duplicates
  if (!_arr->contains(tmp))
    _arr->push_back(tmp);

}


void ShaderGenerator::addInput(QString _input)
{
  addStringToList(_input, &inputs_, "in ", ";");
}



void ShaderGenerator::addOutput(QString _output)
{
  addStringToList(_output, &outputs_, "out ", ";");
}


void ShaderGenerator::addDefine(QString _def)
{
  addStringToList(_def, &genDefines_, "#define ");
}

bool ShaderGenerator::hasDefine(QString _define) const
{
  if (genDefines_.contains(_define))
    return true;

  // check trimmed strings and with startsWith()

  QString trimmedDef = _define.trimmed();

  for (QStringList::const_iterator it = genDefines_.constBegin(); it != genDefines_.constEnd(); ++it)
  {
    QString trimmedRef = it->trimmed();

    if (trimmedRef.startsWith(trimmedDef))
      return true;
  }

  return false;
}

void ShaderGenerator::addLayout(QString _def)
{
  addStringToList(_def, &layouts_);
}


void ShaderGenerator::addUniform(QString _uniform, QString _comment)
{
  QString prefix = "";
  if (!_uniform.startsWith("uniform ") && !_uniform.contains(" uniform "))
    prefix = "uniform ";

  addStringToList(_uniform, &uniforms_, prefix, "; " + _comment );
}



void ShaderGenerator::addIOToCode(const QStringList& _cmds)
{
  QString it;
  foreach(it, _cmds)
  {
    code_.push_back(it);
    // append ; eventually

    if (!it.contains(";"))
      code_.back().append(";");
  }
}



void ShaderGenerator::buildShaderCode(QStringList* _pMainCode, const QStringList& _defaultLightingFunctions)
{
  QString glslversion;
  glslversion.sprintf("#version %d", version_);

  code_.push_back(glslversion);

  // provide defines
  QString it;

  foreach(it, genDefines_)
    code_.push_back(it);

  // layouts
  foreach(it, layouts_)
    code_.push_back(it);

  // IO
  addIOToCode(inputs_);
  addIOToCode(outputs_);
  addIOToCode(uniforms_);

  // eventually attach lighting functions if required
  bool requiresLightingCode = false;

  // search for references in imports
  foreach(it, imports_)
  {
    if (it.contains("LitDirLight") || it.contains("LitPointLight") || it.contains("LitSpotLight"))
      requiresLightingCode = true;
  }

  if (requiresLightingCode)
  {
    foreach(it, _defaultLightingFunctions)
      code_.push_back(it);
  }

  // provide imports
  foreach(it, imports_)
    code_.push_back(it);


  // search for lighting references in main code

  if (!requiresLightingCode)
  {
    foreach(it, (*_pMainCode))
    {
      if (it.contains("LitDirLight") || it.contains("LitPointLight") || it.contains("LitSpotLight"))
        requiresLightingCode = true;
    }

    if (requiresLightingCode)
    {
      foreach(it, _defaultLightingFunctions)
        code_.push_back(it);
    }
  }


  // main function
  foreach(it, (*_pMainCode))
    code_.push_back(it);
}



void ShaderGenerator::addIncludeFile(QString _fileName)
{
  QFile file(_fileName);

  if (file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QTextStream fileStream(&file);
    
    // track source of include files in shader comment
    
    imports_.push_back("// ==============================================================================");
    imports_.push_back(QString("// ShaderGenerator - begin of imported file: ") + _fileName);
    

    while (!fileStream.atEnd())
    {
      QString tmpLine = fileStream.readLine();

      imports_.push_back(tmpLine.simplified());
    }
    
    
    // mark end of include file in comment
    
    imports_.push_back(QString("// ShaderGenerator - end of imported file #include \"") + _fileName);
    imports_.push_back("// ==============================================================================");
    
  }

}



void ShaderGenerator::saveToFile(const char* _fileName)
{
  QFile file(_fileName);
  if (file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    QTextStream fileStream(&file);

    QString it;
    foreach(it, code_)
      fileStream << it << '\n';
  }
}



const QStringList& ShaderGenerator::getShaderCode()
{
  return code_;
}

void ShaderGenerator::setGLSLVersion( int _version )
{
  version_ = _version;
}

void ShaderGenerator::matchInputs(const ShaderGenerator* _previousShaderStage,
  bool _passToNextStage,
  QString _inputPrefix, 
  QString _outputPrefix)
{
  if (!_previousShaderStage)
  {
    std::cout << "error: ShaderGenerator::matchInputs called without providing input stage" << std::endl;
    return;
  }

  QString it;
  foreach(it, _previousShaderStage->outputs_)
  {
    QString input = it;

    QString outKeyword = "out ";
    QString inKeyword = "in  ";

    // replace first occurrence of "out" with "in"
    input.replace(input.indexOf(outKeyword), outKeyword.size(), inKeyword);

    // special case for array IO

    if (inputArrays_ && !_previousShaderStage->outputArrays_)
    {
      QRegExp alphaNum("[a-zA-Z0-9]");
      int lastNameChar = input.lastIndexOf(alphaNum);
      input.insert(lastNameChar+1, "[]");
//      input.insert(lastNameChar+1, "[gl_in.length()]");
    }


    // add to input list with duplicate check
    addStringToList(input, &inputs_);

    if (_passToNextStage)
    {
      // replace prefixes of in/outputs to avoid name collision

      QString output = input;
      output.replace(output.indexOf(_inputPrefix), _inputPrefix.size(), _outputPrefix);
      output.replace(output.indexOf(inKeyword), inKeyword.size(), outKeyword);

      // take care of arrays
      if (inputArrays_ && !outputArrays_)
      {
        int bracketStart = output.indexOf("[");
        int bracketEnd = output.indexOf("]");
        output.remove(bracketStart, bracketEnd-bracketStart+1);
      }
      else if (!inputArrays_ && outputArrays_)
      {
        QRegExp alphaNum("[a-zA-Z0-9]");
        int lastNameChar = output.lastIndexOf(alphaNum);
        output.insert(lastNameChar+1, "[]");
//        output.insert(lastNameChar+1, "[gl_in.length()]");
      }


      // add to output list with duplicate check
      addStringToList(output, &outputs_);
    }
  }
}

int ShaderGenerator::getNumOutputs() const
{
  return outputs_.size();
}

QString ShaderGenerator::getOutputName(int _id) const
{
  QString output = outputs_.at(_id);

  output.remove(";");
  output.remove("out ");

  int bracketStart = output.indexOf("[");
  int bracketEnd = output.lastIndexOf("]");

  if (bracketStart >= 0)
    output.remove(bracketStart, bracketEnd-bracketStart+1);

  // decompose output declaration
  QStringList decompOutput = output.split(" ");
  return decompOutput.last();
}

int ShaderGenerator::getNumInputs() const
{
  return inputs_.size();
}

QString ShaderGenerator::getInputName(int _id) const
{
  QString input = inputs_.at(_id);

  input.remove(";");
  input.remove("out ");
  
  int bracketStart = input.indexOf("[");
  int bracketEnd = input.lastIndexOf("]");

  if (bracketStart >= 0)
    input.remove(bracketStart, bracketEnd-bracketStart+1);

  // decompose output declaration
  QStringList decompInput = input.split(" ");
  return decompInput.last();
}

QString ShaderGenerator::getIOMapName(int _inId) const
{
  QString inputName = getInputName(_inId);

  // output name = input name with swapped prefix
  QString outputName = inputName;
  outputName.replace(outputName.indexOf(inputPrefix_), inputPrefix_.size(), outputPrefix_);

  return outputName;
}

QString ShaderProgGenerator::shaderDir_;
QStringList ShaderProgGenerator::lightingCode_;

ShaderProgGenerator::ShaderProgGenerator(const ShaderGenDesc* _desc,
                                         unsigned int _usage)
: vertex_(0), tessControl_(0), tessEval_(0), geometry_(0), fragment_(0), usage_(_usage),
  inputTexCoord_(false), inputColor_(false), inputNormal_(false),
  passPosVS_(false), passPosOS_(false), passTexCoord_(false), passColor_(false), passNormal_(false)
{
  if (shaderDir_.isEmpty())
    std::cout << "error: call ShaderProgGenerator::setShaderDir() first!" << std::endl;
  else
  {
    desc_ = *_desc;

    // We need at least version 3.2 or higher to support geometry shaders
    if ( !ACG::openGLVersion(3,2) )
      desc_.geometryTemplateFile.clear();

    // We need at least version 4.0 or higher to support tessellation
    if ( !ACG::openGLVersion(4, 0) )
    {
      desc_.tessControlTemplateFile.clear();
      desc_.tessEvaluationTemplateFile.clear();
    }

    // adjust glsl version to requirement

    if (!desc_.geometryTemplateFile.isEmpty())
      desc_.version = std::max(desc_.version, 150);

    if (!desc_.tessControlTemplateFile.isEmpty() || !desc_.tessEvaluationTemplateFile.isEmpty())
      desc_.version = std::max(desc_.version, 400);


    loadLightingFunctions();

    generateShaders();
  }
}

ShaderProgGenerator::~ShaderProgGenerator(void)
{
  delete vertex_;
  delete fragment_;
  delete geometry_;
  delete tessControl_;
  delete tessEval_;
}



void ShaderProgGenerator::loadStringListFromFile(QString _fileName, QStringList* _out)
{
  QFile file(_fileName);

  file.open(QIODevice::ReadOnly | QIODevice::Text);

  if (!file.isReadable())
    std::cout << "error: file missing -> \"" << _fileName.toStdString() << "\"" << std::endl;

  else
  {
    QTextStream filestream(&file);

    while (!filestream.atEnd())
    {
      QString szLine = filestream.readLine();
      _out->push_back(szLine.trimmed());
    }
  }
}


void ShaderProgGenerator::loadLightingFunctions()
{
  if (lightingCode_.size()) return;
  
  QString fileName = shaderDir_ + QDir::separator() + QString(LIGHTING_CODE_FILE);

  lightingCode_.push_back("// ==============================================================================");
  lightingCode_.push_back(QString("// ShaderGenerator - default lighting functions imported from file: ") + fileName);
  
  
  // load shader code from file
  loadStringListFromFile(fileName, &lightingCode_);
  
  lightingCode_.push_back(QString("// ShaderGenerator - end of default lighting functions"));
  lightingCode_.push_back("// ==============================================================================");
}



void ShaderProgGenerator::initGenDefines(ShaderGenerator* _gen)
{
  switch (desc_.shadeMode)
  {
  case SG_SHADE_GOURAUD:
    _gen->addDefine("SG_GOURAUD 1"); break;
  case SG_SHADE_FLAT:
    _gen->addDefine("SG_FLAT 1"); break;
  case SG_SHADE_UNLIT:
    _gen->addDefine("SG_UNLIT 1"); break;
  case SG_SHADE_PHONG:
    _gen->addDefine("SG_PHONG 1"); break;

  default:
    std::cout << __FUNCTION__ << " -> unknown shade mode: " << desc_.shadeMode << std::endl;
  }

  if (desc_.textured())
    _gen->addDefine("SG_TEXTURED 1");

  if (desc_.vertexColors)
    _gen->addDefine("SG_VERTEX_COLOR 1");

//  if (desc_.shadeMode != SG_SHADE_UNLIT)
  if (passNormal_)
    _gen->addDefine("SG_NORMALS 1");

  if (passPosVS_)
    _gen->addDefine("SG_POSVS 1");

  if (passPosOS_)
    _gen->addDefine("SG_POSOS 1");

  // # lights define
  QString strNumLights;
  strNumLights.sprintf("SG_NUM_LIGHTS %d", desc_.numLights);
  _gen->addDefine(strNumLights);

  // light types define
  const char* lightTypeNames[] = {"SG_LIGHT_DIRECTIONAL",
    "SG_LIGHT_POINT", "SG_LIGHT_SPOT"};

  for (int i = 0; i < 3; ++i)
    _gen->addDefine(lightTypeNames[i]);


  for (int i = 0; i < desc_.numLights; ++i)
  {
    QString strLightType;
    strLightType.sprintf("SG_LIGHT_TYPE_%d %s", i, lightTypeNames[desc_.lightTypes[i]]);
    _gen->addDefine(strLightType);
  }

  _gen->addDefine("SG_ALPHA g_vMaterial.y");
}




void ShaderProgGenerator::buildVertexShader()
{
  delete vertex_;

  vertex_  = new ShaderGenerator();
  vertex_->setGLSLVersion(desc_.version);

//  vertex_->initDefaultVertexShaderIO();
  vertex_->initVertexShaderIO(&desc_, passPosVS_, passPosOS_, passNormal_, passTexCoord_, passColor_);

  vertex_->initDefaultUniforms();


  // apply i/o modifiers
  for (int i = 0; i < numModifiers_; ++i)
  {
    if (usage_ & (1 << i))
      modifiers_[i]->modifyVertexIO(vertex_);
  }


  initGenDefines(vertex_);



  // IO

  // when to use vertex lights
  if (desc_.shadeMode == SG_SHADE_GOURAUD ||
    desc_.shadeMode == SG_SHADE_FLAT)
  {
    for (int i = 0; i < desc_.numLights; ++i)
      vertex_->addLight(i, desc_.lightTypes[i]);
  }


  // assemble main function
  QStringList mainCode;

  if (!vertexTemplate_.size())
  {
    mainCode.push_back("void main()");
    mainCode.push_back("{");

    addVertexBeginCode(&mainCode);
    addVertexEndCode(&mainCode);

    mainCode.push_back("}");
  }
  else
  {
    // interpret loaded shader template:
    //  import #includes and replace SG_VERTEX_BEGIN/END markers

    QString it;
    foreach(it,vertexTemplate_)
    {
      if (!checkForIncludes(it, vertex_, getPathName(vertexShaderFile_)))
      {
        // str line is no include directive
        // check for SG_ markers

        if (it.contains("SG_VERTEX_BEGIN"))
          addVertexBeginCode(&mainCode);
        else
        {
          if (it.contains("SG_VERTEX_END"))
            addVertexEndCode(&mainCode);
          else
          {
            // no SG marker
            mainCode.push_back(it);
          }
        }

      }
    }

  }

  vertex_->buildShaderCode(&mainCode, lightingCode_);

}


void ShaderProgGenerator::addVertexBeginCode(QStringList* _code)
{
  // size in pixel of rendered point-lists, set by user via uniform

  _code->push_back("vec4 sg_vPosPS = g_mWVP * inPosition;");
  _code->push_back("vec4 sg_vPosVS = g_mWV * inPosition;");
  _code->push_back("vec3 sg_vNormalVS = vec3(0.0, 1.0, 0.0);");

  /// TODO Setup for multiple texture coordinates as input
  if (desc_.textured())
  {
    if (desc_.textureTypes().begin()->second.type == GL_TEXTURE_3D) {
      _code->push_back("vec3 sg_vTexCoord = vec3(0.0, 0.0, 0.0);");
    } else {
      _code->push_back("vec2 sg_vTexCoord = vec2(0.0, 0.0);");
    }
  }

  _code->push_back("vec4 sg_cColor = vec4(g_cEmissive, SG_ALPHA);");

  if (desc_.shadeMode != SG_SHADE_UNLIT)
    _code->push_back("sg_vNormalVS = normalize(g_mWVIT * inNormal);");

  if (desc_.textured())
    _code->push_back("sg_vTexCoord = inTexCoord;");

  if (desc_.vertexColors)
    _code->push_back("sg_cColor = inColor;");

  if (desc_.shadeMode == SG_SHADE_GOURAUD ||
    desc_.shadeMode == SG_SHADE_FLAT)
  {
    // add lighting code here

    addLightingCode(_code);
  }

  if (passPosOS_)
    _code->push_back("outVertexPosOS = inPosition;");

  if (passPosVS_)
    _code->push_back("outVertexPosVS = sg_vPosVS;");

  // apply modifiers
  for (int i = 0; i < numModifiers_; ++i)
  {
    if (usage_ & (1 << i))
      modifiers_[i]->modifyVertexBeginCode(_code);
  }
}


void ShaderProgGenerator::addVertexEndCode(QStringList* _code)
{
  _code->push_back("gl_Position = sg_vPosPS;");
  _code->push_back("outVertexPosCS = sg_vPosPS;");

  if (desc_.textured())
    _code->push_back("outVertexTexCoord = sg_vTexCoord;");

  if (desc_.shadeMode == SG_SHADE_GOURAUD ||
    desc_.shadeMode == SG_SHADE_FLAT ||
    desc_.vertexColors)
    _code->push_back("outVertexColor = sg_cColor;");

  if (desc_.shadeMode == SG_SHADE_PHONG)
  {
    _code->push_back("outVertexNormal = sg_vNormalVS;");
    _code->push_back("outVertexPosVS  = sg_vPosVS;");
  }



  // apply modifiers
  for (int i = 0; i < numModifiers_; ++i)
  {
    if (usage_ & (1 << i))
      modifiers_[i]->modifyVertexEndCode(_code);
  }
}


int ShaderProgGenerator::checkForIncludes(QString _str, ShaderGenerator* _gen, QString _includePath)
{
  if (_str.contains("#include "))
  {
    QString strIncludeFile = _str.remove("#include ").remove('\"').remove('<').remove('>').trimmed();

    if (strIncludeFile.isEmpty())
      std::cout << "wrong include syntax: " << _str.toStdString() << std::endl;
    else
    {
      QString fullPathToIncludeFile = _includePath + QDir::separator() + strIncludeFile;

      _gen->addIncludeFile(fullPathToIncludeFile);
    }

    return 1;
  }

  return 0;
}

int ShaderProgGenerator::checkForIncludes(QString _str, QStringList* _outImport, QString _includePath)
{
  if (_str.contains("#include "))
  {
    QString strIncludeFile = _str.remove("#include ").remove('\"').remove('<').remove('>').trimmed();

    if (strIncludeFile.isEmpty())
      std::cout << "wrong include syntax: " << _str.toStdString() << std::endl;
    else
    {
      QString fullPathToIncludeFile = _includePath + QDir::separator() + strIncludeFile;

      loadStringListFromFile(fullPathToIncludeFile, _outImport);
    }

    return 1;
  }

  return 0;
}

void ShaderProgGenerator::buildTessControlShader()
{
  // Only build a tess-control shader if enabled
  if ( desc_.tessControlTemplateFile.isEmpty() )
    return;

  // the generator provides an IO mapping function and adds default uniforms to this stage
  // - template is necessary
  // - combination/modification of tess-control shader is not supported
  // - template may call sg_MapIO(inId, gl_InvocationID) somewhere in code to take care of default IO pass-through
  //         inId can be gl_InvocationID if the patch size is not modified

  delete tessControl_;

  tessControl_  = new ShaderGenerator();
  tessControl_->setGLSLVersion(desc_.version);

  QString it;
  foreach(it, tessControlLayout_)
    tessControl_->addLayout(it);

  // find previous shader stage
  ShaderGenerator* prevStage = vertex_;

  tessControl_->initTessControlShaderIO(&desc_, prevStage);

  tessControl_->initDefaultUniforms();


  // apply i/o modifiers
  for (int i = 0; i < numModifiers_; ++i)
  {
    if (usage_ & (1 << i))
      modifiers_[i]->modifyTessControlIO(tessControl_);
  }

  initGenDefines(tessControl_);



  // assemble main function
  QStringList mainCode; 

  // add simple io passthrough mapper

  {
    mainCode.push_back("void sg_MapIO(const int inIdx, const int outIdx)");
    mainCode.push_back("{");

    // built-in IO
    mainCode.push_back("gl_out[outIdx].gl_Position = gl_in[inIdx].gl_Position;");
    
    // custom IO
    for (int i = 0; i < tessControl_->getNumInputs(); ++i)
    {
      QString inputName = tessControl_->getInputName(i);
      QString outputName = tessControl_->getIOMapName(i);

      QString outputAssignCode = outputName + QString("[outIdx] = ") + inputName + QString("[inIdx];");

      mainCode.push_back(outputAssignCode);
    }

    mainCode.push_back("}");
  }


  // interpret loaded shader template:
  //  import #includes
  foreach(it,tessControlTemplate_)
  {
    if (!checkForIncludes(it, tessControl_, getPathName(tessControlShaderFile_)))
    {
      // str line is no include directive
      mainCode.push_back(it);
    }
  }

  tessControl_->buildShaderCode(&mainCode, lightingCode_);
}

void ShaderProgGenerator::buildTessEvalShader()
{
  // Only build a tess-eval shader if enabled
  if ( desc_.tessEvaluationTemplateFile.isEmpty() )
    return;

  // the generator provides default interpolation functions and adds default uniforms to this stage
  // - template is necessary
  // - combination/modification of tess-eval shader is not supported
  // - template may call sg_MapIOBarycentric() or sg_MapIOBilinear() somewhere in code to take care of default IO pass-through
  //        - barycentric interpolation can be used for triangle patches
  //        - bilinear interpolation can be used for quad patches
  //        - other interpolation schemes have to be user defined

  delete tessEval_;

  tessEval_  = new ShaderGenerator();
  tessEval_->setGLSLVersion(desc_.version);


  // find previous shader stage
  ShaderGenerator* prevStage = tessControl_;

  if (!prevStage)
    prevStage = vertex_;

  tessEval_->initTessEvalShaderIO(&desc_, prevStage);

  tessEval_->initDefaultUniforms();

  QString itLayout;
  foreach(itLayout, tessEvalLayout_)
    tessEval_->addLayout(itLayout);

  // apply i/o modifiers
  for (int i = 0; i < numModifiers_; ++i)
  {
    if (usage_ & (1 << i))
      modifiers_[i]->modifyTessControlIO(tessEval_);
  }

  initGenDefines(tessEval_);


  // assemble main function
  QStringList mainCode; 

  // add simple io passthrough mapper

  {
    // barycentric interpolation

    mainCode.push_back("void sg_MapIOBarycentric()");
    mainCode.push_back("{");

    // built-in IO
    mainCode.push_back("gl_Position = gl_TessCoord.x * gl_in[0].gl_Position + gl_TessCoord.y * gl_in[1].gl_Position + gl_TessCoord.z * gl_in[2].gl_Position;");

    // custom IO
    for (int i = 0; i < tessEval_->getNumInputs(); ++i)
    {
      QString inputName = tessEval_->getInputName(i);
      QString outputName = tessEval_->getIOMapName(i);

      QString outputAssignCode = outputName + QString(" = ") +
        QString("gl_TessCoord.x*") + inputName + QString("[0] + ") +
        QString("gl_TessCoord.y*") + inputName + QString("[1] + ") +
        QString("gl_TessCoord.z*") + inputName + QString("[2];");

      mainCode.push_back(outputAssignCode);
    }

    mainCode.push_back("}");

    
    // bilinear interpolation

    mainCode.push_back("void sg_MapIOBilinear()");
    mainCode.push_back("{");

    // built-in IO
    mainCode.push_back("gl_Position = mix(  mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x), mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x), gl_TessCoord.y);");

    // custom IO
    for (int i = 0; i < tessEval_->getNumInputs(); ++i)
    {
      QString inputName = tessEval_->getInputName(i);
      QString outputName = tessEval_->getIOMapName(i);

      QString outputAssignCode = outputName + QString(" = mix( ") +
        QString("mix(") + inputName + QString("[0], ") + inputName + QString("[1], gl_TessCoord.x), ") + 
        QString("mix(") + inputName + QString("[2], ") + inputName + QString("[3], gl_TessCoord.x), gl_TessCoord.y); ");

      mainCode.push_back(outputAssignCode);
    }

    mainCode.push_back("}");
  }


  // interpret loaded shader template:
  //  replace (SG_INPUT, SG_OUTPUT) with matching io pairs
  QStringList::iterator it;
  for (it = tessEvalTemplate_.begin(); it != tessEvalTemplate_.end(); ++it)
  {
    QString line = *it;

    // replace IO line matching the pattern:
    //  SG_OUTPUT = r_expression(SG_INPUT);
    // the complete expression must be contained in a single line for this to work
    // more complex interpolation code should use #if SG_NORMALS etc.

    if (line.contains("SG_INPUT") || line.contains("SG_OUTPUT"))
    {
      
      mainCode.push_back("// ----------------------------------------");
      mainCode.push_back("// ShaderGen: resolve SG_OUTPUT = expression(SG_INPUT);");

      for (int i = 0; i < tessEval_->getNumInputs(); ++i)
      {
        QString inputName = tessEval_->getInputName(i);
        QString outputName = tessEval_->getIOMapName(i);

        // replace SG_INPUT, SG_OUTPUT with actual names
        QString resolvedLine = line;

        resolvedLine.replace("SG_INPUT", inputName);
        resolvedLine.replace("SG_OUTPUT", outputName);

        mainCode.push_back(resolvedLine);
      }

      mainCode.push_back("// ----------------------------------------");

    }
    else
      mainCode.push_back(line);
  }

  tessEval_->buildShaderCode(&mainCode, lightingCode_);
}

void ShaderProgGenerator::buildGeometryShader()
{
  // Only build a geometry shader if enabled
  if ( desc_.geometryTemplateFile.isEmpty() )
    return;


  delete geometry_;

  geometry_  = new ShaderGenerator();
  geometry_->setGLSLVersion(desc_.version);


  // find previous shader stage
  ShaderGenerator* prevStage = tessEval_;

  if (!prevStage)
    prevStage = vertex_;

  geometry_->initGeometryShaderIO(&desc_, prevStage);

  geometry_->initDefaultUniforms();


  // apply i/o modifiers
  for (int i = 0; i < numModifiers_; ++i)
  {
    if (usage_ & (1 << i))
      modifiers_[i]->modifyGeometryIO(geometry_);
  }

  initGenDefines(geometry_);


  // assemble main function
  QStringList mainCode; 

  // add simple io passthrough mapper

  {
    mainCode.push_back("void sg_MapIO(const int inIdx)");
    mainCode.push_back("{");

    // built-in IO
    mainCode.push_back("gl_Position = gl_in[inIdx].gl_Position;");
    mainCode.push_back("gl_PrimitiveID = gl_PrimitiveIDIn;");

    // custom IO
    for (int i = 0; i < geometry_->getNumInputs(); ++i)
    {
      QString inputName = geometry_->getInputName(i);
      QString outputName = geometry_->getIOMapName(i);

      QString outputAssignCode = outputName + QString(" = ") + inputName + QString("[inIdx];");

      mainCode.push_back(outputAssignCode);
    }

    mainCode.push_back("}");
  }


  // interpret loaded shader template:
  //  import #includes
  QString it;
  foreach(it,geometryTemplate_)
  {
    if (!checkForIncludes(it, geometry_, getPathName(geometryShaderFile_)))
    {
      // str line is no include directive
      mainCode.push_back(it);
    }
  }

  geometry_->buildShaderCode(&mainCode, lightingCode_);
}


void ShaderProgGenerator::buildFragmentShader()
{
  delete fragment_;

  fragment_  = new ShaderGenerator();
  fragment_->setGLSLVersion(desc_.version);

  // find previous shader stage
  ShaderGenerator* prevStage = geometry_;

  if (!prevStage)
    prevStage = tessEval_;
  if (!prevStage)
    prevStage = tessControl_;
  if (!prevStage)
    prevStage = vertex_;


  fragment_->initFragmentShaderIO(&desc_, prevStage);


  fragment_->initDefaultUniforms();


  // texture sampler id
  if (desc_.textured())
  {
    for (std::map<size_t,ShaderGenDesc::TextureType>::const_iterator iter = desc_.textureTypes().begin();
        iter != desc_.textureTypes().end(); ++iter)
    {
      QString name = QString("g_Texture%1").arg(iter->first);
      QString type = "";
      switch (iter->second.type)
      {
        case GL_TEXTURE_1D: type = "sampler1D"; break;
        case GL_TEXTURE_2D: type = "sampler2D"; break;
        case GL_TEXTURE_3D: type = "sampler3D"; break;
        case GL_TEXTURE_CUBE_MAP: type = "samplerCube​"; break;
#ifdef GL_ARB_texture_rectangle //ARCH_DARWIN doesn't support all texture defines with all xcode version (xcode 5.0 seems to support all)
        case GL_TEXTURE_RECTANGLE_ARB: type = "sampler2DRect"; break;
#endif
#ifdef GL_ARB_texture_buffer_object
        case GL_TEXTURE_BUFFER_ARB: type = "samplerBuffer​"; break;
#endif
#ifdef GL_EXT_texture_array
        case GL_TEXTURE_1D_ARRAY_EXT: type = "sampler1DArray"; break;
        case GL_TEXTURE_2D_ARRAY_EXT: type = "sampler2DArray"; break;
#endif
#ifdef GL_ARB_texture_cube_map_array
        case GL_TEXTURE_CUBE_MAP_ARRAY_ARB: type = "samplerCubeArray"; break;
#endif
#ifdef GL_ARB_texture_multisample
        case GL_TEXTURE_2D_MULTISAMPLE: type = "sampler2DMS"; break;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY: type = "sampler2DMSArray​"; break;
#endif
        default: std::cerr << "Texture Type not supported "<< iter->second.type << std::endl; break;
      }
      // todo: check if texture type supports shadowtype
      if (iter->second.shadow)
        type += "Shadow";
      fragment_->addUniform(type + " " + name);
    }
  }

  // apply i/o modifiers
  for (int i = 0; i < numModifiers_; ++i)
  {
    if (usage_ & (1 << i))
      modifiers_[i]->modifyFragmentIO(fragment_);
  }


  initGenDefines(fragment_);



  // io

  // when to use fragment lights
  if (desc_.shadeMode == SG_SHADE_PHONG)
  {
    for (int i = 0; i < desc_.numLights; ++i)
      fragment_->addLight(i, desc_.lightTypes[i]);
  }

  // assemble main function
  QStringList mainCode;

  if (!fragmentTemplate_.size())
  {
    mainCode.push_back("void main()");
    mainCode.push_back("{");

    addFragmentBeginCode(&mainCode);
    addFragmentEndCode(&mainCode);

    mainCode.push_back("}");
  }
  else
  {
    // interpret loaded shader template:
    //  import #includes and replace SG_VERTEX_BEGIN/END markers
    QString it;
    foreach(it,fragmentTemplate_)
    {
      if (!checkForIncludes(it, fragment_, getPathName(fragmentShaderFile_)))
      {
        // str line is no include directive
        // check for SG_ markers

        if (it.contains("SG_FRAGMENT_BEGIN"))
          addFragmentBeginCode(&mainCode);
        else
        {
          if (it.contains("SG_FRAGMENT_END"))
            addFragmentEndCode(&mainCode);
          else
          {
            // no SG marker
            mainCode.push_back(it);
          }
        }

      }

      
    }

  }



  fragment_->buildShaderCode(&mainCode, lightingCode_);
}


void ShaderProgGenerator::addFragmentBeginCode(QStringList* _code)
{

  QString inputShader = "Vertex";

  if ( tessEval_ )
    inputShader = "Te";
  if ( geometry_ )
    inputShader = "Geometry";

  // support for projective texture mapping
  _code->push_back("vec4 sg_vPosCS = out" + inputShader + "PosCS;");
  _code->push_back("vec2 sg_vScreenPos = out" + inputShader + "PosCS.xy / out" + inputShader + "PosCS.w * 0.5 + vec2(0.5, 0.5);");

  _code->push_back("vec4 sg_cColor = vec4(g_cEmissive, SG_ALPHA);");

  if (desc_.shadeMode == SG_SHADE_GOURAUD ||
      desc_.shadeMode == SG_SHADE_FLAT    ||
      desc_.vertexColors)
  {
    _code->push_back("sg_cColor = out" + inputShader + "Color;");
  }


  if (desc_.shadeMode == SG_SHADE_PHONG)
  {
    _code->push_back("vec4 sg_vPosVS = out" + inputShader + "PosVS;");
    _code->push_back("vec3 sg_vNormalVS = out" + inputShader + "Normal;");


    addLightingCode(_code);
  }

  if (desc_.textured())
  {
    std::map<size_t,ShaderGenDesc::TextureType>::const_iterator iter = desc_.textureTypes().begin();
    _code->push_back("vec4 sg_cTex = texture(g_Texture"+QString::number(iter->first)+", out" + inputShader + "TexCoord);");

    for (++iter; iter != desc_.textureTypes().end(); ++iter)
      _code->push_back("sg_cTex += texture(g_Texture"+QString::number(iter->first)+", out" + inputShader + "TexCoord);");

    if (desc_.textureTypes().size() > 1 && desc_.normalizeTexColors)
      _code->push_back("sg_cTex = sg_cTex * 1.0/" + QString::number(desc_.textureTypes().size()) +".0 ;");

    if (desc_.shadeMode == SG_SHADE_UNLIT)
      _code->push_back("sg_cColor += sg_cTex;");
    else
      _code->push_back("sg_cColor *= sg_cTex;");
  }

  
  // apply modifiers
  for (int i = 0; i < numModifiers_; ++i)
  {
    if (usage_ & (1 << i))
      modifiers_[i]->modifyFragmentBeginCode(_code);
  }
}

void ShaderProgGenerator::addFragmentEndCode(QStringList* _code)
{
  _code->push_back("outFragment = sg_cColor;");

  // apply modifiers
  for (int i = 0; i < numModifiers_; ++i)
  {
    if (usage_ & (1 << i))
      modifiers_[i]->modifyFragmentEndCode(_code);
  }
}



void ShaderProgGenerator::addLightingCode(QStringList* _code)
{

  ShaderModifier* lightingModifier = 0;

  // check if any modifier replaces the default lighting function
  for (int i = 0; i < numModifiers_ && !lightingModifier; ++i)
  {
    if (usage_ & (1 << i))
      if (modifiers_[i]->replaceDefaultLightingCode())
        lightingModifier = modifiers_[i];
  }

  if (!lightingModifier)
  {
    // default lighting code:

    QString buf;

    for (int i = 0; i < desc_.numLights; ++i)
    {
      ShaderGenLightType lgt = desc_.lightTypes[i];

      switch (lgt)
      {
      case SG_LIGHT_DIRECTIONAL:
        buf.sprintf("sg_cColor.xyz += LitDirLight(sg_vPosVS.xyz, sg_vNormalVS, g_vLightDir_%d,  g_cLightAmbient_%d,  g_cLightDiffuse_%d,  g_cLightSpecular_%d);", i, i, i, i);
        break;

      case SG_LIGHT_POINT:
        buf.sprintf("sg_cColor.xyz += LitPointLight(sg_vPosVS.xyz, sg_vNormalVS,  g_vLightPos_%d,  g_cLightAmbient_%d,  g_cLightDiffuse_%d,  g_cLightSpecular_%d,  g_vLightAtten_%d);", i, i, i, i, i);
        break;

      case SG_LIGHT_SPOT:
        buf.sprintf("sg_cColor.xyz += LitSpotLight(sg_vPosVS.xyz,  sg_vNormalVS,  g_vLightPos_%d,  g_vLightDir_%d,  g_cLightAmbient_%d,  g_cLightDiffuse_%d,  g_cLightSpecular_%d,  g_vLightAtten_%d,  g_vLightAngleExp_%d);", i, i, i, i, i, i, i);
        break;

      default: break;
      }

      _code->push_back(buf);
    }

    // modify lighting color afterwards

    for (int i = 0; i < numModifiers_; ++i)
    {
      if (usage_ & (1 << i))
        modifyLightingCode(_code, modifiers_[i]);
    }
  }
  else
  {
    // there exists a lighting modifier that completely replaces the default lighting shader
    modifyLightingCode(_code, lightingModifier);


    // apply remaining modifiers that do not replace the complete lighting code

    for (int i = 0; i < numModifiers_; ++i)
    {
      if (usage_ & (1 << i) && lightingModifier != modifiers_[i])
        modifyLightingCode(_code, modifiers_[i]);
    }
  }

}

void ShaderProgGenerator::modifyLightingCode( QStringList* _code, ShaderModifier* _modifier )
{
  if (!_modifier) return;

  for (int i = 0; i < desc_.numLights; ++i)
  {
    ShaderGenLightType lgt = desc_.lightTypes[i];

    _modifier->modifyLightingCode(_code, i, lgt);
  }
}


void ShaderProgGenerator::addLightingFunctions(QStringList* _code)
{
  QString it;
  foreach(it,lightingCode_)
    _code->push_back(it);
}

void ShaderProgGenerator::generateShaders()
{
  // import template source from files
  loadShaderTemplateFromFile();

  // check what needs to be passed down from vertex shader

  if (desc_.shadeMode != SG_SHADE_UNLIT)
    inputNormal_ = true;

  if (desc_.textured())
  {
    inputTexCoord_ = true;
    passTexCoord_ = true;
  }

  if (desc_.vertexColors)
    inputColor_ = true;

  if (desc_.shadeMode == SG_SHADE_PHONG)
  {
    passNormal_ = true;
    passPosVS_ = true;
  }

  if (desc_.shadeMode == SG_SHADE_FLAT || desc_.shadeMode == SG_SHADE_GOURAUD || desc_.vertexColors)
    passColor_ = true;


  // scan macros of modifiers for attribute requests,
  // done by adding modifier io to an empty dummy
  ShaderGenerator dummy;

  for (int i = 0; i < numModifiers_; ++i)
  {
    if (usage_ & (1 << i))
    {
      ShaderModifier* mod = modifiers_[i];

      mod->modifyVertexIO(&dummy);
      mod->modifyTessControlIO(&dummy);
      mod->modifyTessEvalIO(&dummy);
      mod->modifyGeometryIO(&dummy);
      mod->modifyFragmentIO(&dummy);
    }
  }
  // scan requested inputs from modifiers

  if (dummy.hasDefine(SG_REQUEST_POSVS))
    passPosVS_ = true;
  if (dummy.hasDefine(SG_REQUEST_TEXCOORD))
  {
    inputTexCoord_ = true;
    passTexCoord_ = true;
  }
  if (dummy.hasDefine(SG_REQUEST_VERTEXCOLOR))
  {
    inputColor_ = true;
    passColor_ = true;
  }
  if (dummy.hasDefine(SG_REQUEST_NORMAL))
  {
    inputNormal_ = true;
    passNormal_ = true;
  }
  if (dummy.hasDefine(SG_REQUEST_POSOS))
    passPosOS_ = true;




  // assemble shader codes

  buildVertexShader();
  buildTessControlShader();
  buildTessEvalShader();
  buildGeometryShader();
  buildFragmentShader();
}


const QStringList& ShaderProgGenerator::getVertexShaderCode()
{
  return vertex_->getShaderCode();
}

const QStringList& ShaderProgGenerator::getTessControlShaderCode()
{
  return tessControl_->getShaderCode();
}

const QStringList& ShaderProgGenerator::getTessEvaluationShaderCode()
{
  return tessEval_->getShaderCode();
}

const QStringList& ShaderProgGenerator::getGeometryShaderCode()
{
  return geometry_->getShaderCode();
}

const QStringList& ShaderProgGenerator::getFragmentShaderCode()
{
  return fragment_->getShaderCode();
}


void ShaderProgGenerator::saveVertexShToFile(const char* _fileName)
{
  vertex_->saveToFile(_fileName);
}

void ShaderProgGenerator::saveGeometryShToFile(const char* _fileName)
{
  geometry_->saveToFile(_fileName);
}

void ShaderProgGenerator::saveFragmentShToFile(const char* _fileName)
{
  fragment_->saveToFile(_fileName);
}


void ShaderProgGenerator::loadShaderTemplateFromFile()
{
  if (!desc_.vertexTemplateFile.isEmpty())
  {
    loadStringListFromFile(desc_.vertexTemplateFile, &vertexTemplate_);
    scanShaderTemplate(vertexTemplate_, desc_.vertexTemplateFile);
  }
  if (!desc_.fragmentTemplateFile.isEmpty())
  {
    loadStringListFromFile(desc_.fragmentTemplateFile, &fragmentTemplate_);
    scanShaderTemplate(fragmentTemplate_, desc_.fragmentTemplateFile);
  }
  if (!desc_.geometryTemplateFile.isEmpty())
  {
    loadStringListFromFile(desc_.geometryTemplateFile, &geometryTemplate_);
    scanShaderTemplate(geometryTemplate_, desc_.geometryTemplateFile);
  }
  if (!desc_.tessControlTemplateFile.isEmpty())
  {
    loadStringListFromFile(desc_.tessControlTemplateFile, &tessControlTemplate_);
    scanShaderTemplate(tessControlTemplate_, desc_.tessControlTemplateFile, &tessControlLayout_);
  }
  if (!desc_.tessEvaluationTemplateFile.isEmpty())
  {
    loadStringListFromFile(desc_.tessEvaluationTemplateFile, &tessEvalTemplate_);
    scanShaderTemplate(tessEvalTemplate_, desc_.tessEvaluationTemplateFile, &tessEvalLayout_);
  }


  vertexShaderFile_   = desc_.vertexTemplateFile;
  tessControlShaderFile_ = desc_.tessControlTemplateFile;
  tessEvalShaderFile_ = desc_.tessEvaluationTemplateFile;
  geometryShaderFile_ = desc_.geometryTemplateFile;
  fragmentShaderFile_ = desc_.fragmentTemplateFile;
}

void ShaderProgGenerator::scanShaderTemplate(QStringList& _templateSrc, QString _templateFilename, QStringList* _outLayoutDirectives)
{
  // interpret loaded shader template:
  //  import #includes

  QString filePath = getPathName(_templateFilename);

  QStringList::iterator it;
  for (it = _templateSrc.begin(); it != _templateSrc.end(); ++it)
  {
    QStringList import;

    if (checkForIncludes(*it, &import, filePath))
    {
      // line is include directive

      // remove line from source
      it = _templateSrc.erase(it);

      int offset = it - _templateSrc.begin();

      // insert imported file

      QString importLine;
      foreach(importLine, import)
      {
        it = _templateSrc.insert(it, importLine);
        ++it;
      }

      // included file might recursively include something again
      // -> scan included file
      it = _templateSrc.begin() + offset;
    }
    else
    {
      QString trimmedLine = it->trimmed();

      // scan and adjust glsl version
      QByteArray lineBytes = trimmedLine.toUtf8();

      int templateVersion = 0;
      if (sscanf(lineBytes.constData(), "#version %d", &templateVersion) == 1)
      {
        desc_.version = std::max(templateVersion, desc_.version);

        // remove version line from template since this is added later in the build functions
        it = _templateSrc.erase(it);
      }
      // scan layout() directive
      else if (trimmedLine.startsWith("layout(") || trimmedLine.startsWith("layout ("))
      {
        if (_outLayoutDirectives)
        {
          _outLayoutDirectives->push_back(trimmedLine);
          // layout() will be inserted later at the correct position in the build functions
          // - must be placed before shader IO declaration to make tess-control shaders compilable on ati
          it = _templateSrc.erase(it);
        }
      }
      else
      {
        // scan requested inputs

        if (trimmedLine.startsWith(SG_REQUEST_POSVS))
          passPosVS_ = true;
        else if (trimmedLine.startsWith(SG_REQUEST_TEXCOORD))
        {
          inputTexCoord_ = true;
          passTexCoord_ = true;
        }
        else if (trimmedLine.startsWith(SG_REQUEST_VERTEXCOLOR))
        {
          inputColor_ = true;
          passColor_ = true;
        }
        else if (trimmedLine.startsWith(SG_REQUEST_NORMAL))
        {
          inputNormal_ = true;
          passNormal_ = true;
        }
        else if (trimmedLine.startsWith(SG_REQUEST_POSOS))
          passPosOS_ = true;
      }

    }
  }

}

QString ShaderProgGenerator::getPathName(QString _strFileName)
{
  QFileInfo fileInfo(_strFileName);
  return fileInfo.absolutePath();
}

void ShaderProgGenerator::setShaderDir( QString _dir )
{
  shaderDir_ = _dir;
}

QString ShaderProgGenerator::getShaderDir()
{
  return shaderDir_ + QString("/");
}


unsigned int ShaderProgGenerator::registerModifier( ShaderModifier* _modifier )
{
  if (!_modifier) return 0;

  // redundancy check
  for (unsigned int i = 0; i < 32; ++i)
  {
    if (modifiers_[i] == _modifier) 
      return i;
  }

  if (numModifiers_ == 32) 
  {
    std::cout << "warning: exceeded maximal ShaderModifier count!" << std::endl;
    return 0;
  }

  _modifier->modifierID_ = (unsigned int)(1 << numModifiers_);

  modifiers_[numModifiers_++] = _modifier;
  return _modifier->modifierID_;
}

ShaderModifier* ShaderProgGenerator::getActiveModifier( int _i )
{
  // search active modifiers
  int counter = 0;
  for (int i = 0; i < numModifiers_; ++i)
  {
    if (usage_ & (1 << i))
    {
      if (counter++ == _i)
        return modifiers_[i];
    }
  }

  // invalid _i
  return 0;
}

int ShaderProgGenerator::getNumActiveModifiers() const
{
  // count modifiers
  int numActive = 0;
  for (int i = 0; i < numModifiers_; ++i)
  {
    if (usage_ & (1 << i))
      ++numActive;
  }
  return numActive;
}


bool ShaderProgGenerator::hasGeometryShader() const
{
  return !desc_.geometryTemplateFile.isEmpty();
}

bool ShaderProgGenerator::hasTessControlShader() const
{
  return !desc_.tessControlTemplateFile.isEmpty();
}

bool ShaderProgGenerator::hasTessEvaluationShader() const
{
  return !desc_.tessEvaluationTemplateFile.isEmpty();
}


//=============================================================================

ShaderModifier::ShaderModifier( void )
: modifierID_(0)
{}

ShaderModifier::~ShaderModifier( void )
{}

//=============================================================================


QString ShaderGenDesc::toString() const
{
  // mapping (int)ShaderGenMode -> string
  const char* shadeModeString[] = 
  {
    "SG_SHADE_UNLIT",
    "SG_SHADE_FLAT",
    "SG_SHADE_GOURAUD",
    "SG_SHADE_PHONG"
  };

  QString res;
  QTextStream resStrm(&res);

  resStrm << "version: " << version;

  resStrm << "shaderDesc.numLights: " << numLights;

  if (numLights)
  {
    resStrm << "\nshaderDesc.lightTypes[]: {";

    for (int i = 0; i < numLights; ++i)
    {
      switch (lightTypes[i]) 
      {
      case SG_LIGHT_DIRECTIONAL: resStrm << "DIRECTIONAL"; break;
      case SG_LIGHT_POINT: resStrm << "POINT"; break;
      case SG_LIGHT_SPOT: resStrm << "SPOT"; break;
      default: resStrm << "UNDEFINED"; break;
      }

      if (i + 1 < numLights)
        resStrm << ", ";
      else
        resStrm << "}";
    }
  }


  resStrm << "\nshaderDesc.shadeMode: " << shadeModeString[shadeMode];
  resStrm << "\nshaderDesc.vertexColors: " << vertexColors;
  resStrm << "\nshaderDesc.textured(): " << textured();
  for (std::map<size_t,TextureType>::const_iterator iter = textureTypes_.begin(); iter != textureTypes_.end();++iter)
  {
    resStrm << "\nTexture stage: " << iter->first;
    resStrm << "\nTexture Type: ";
    switch (iter->second.type)
    {
        case GL_TEXTURE_1D: resStrm << "GL_TEXTURE_1D"; break;
        case GL_TEXTURE_2D: resStrm << "GL_TEXTURE_2D"; break;
        case GL_TEXTURE_3D: resStrm << "GL_TEXTURE_3D"; break;
        case GL_TEXTURE_CUBE_MAP: resStrm << "GL_TEXTURE_CUBE_MAP​"; break;
#ifdef GL_ARB_texture_rectangle //ARCH_DARWIN doesn't support all texture defines with all xcode version (xcode 5.0 seems to support all)
        case GL_TEXTURE_RECTANGLE_ARB: resStrm << "GL_TEXTURE_RECTANGLE"; break;
#endif
#ifdef GL_ARB_texture_buffer_object
        case GL_TEXTURE_BUFFER_ARB: resStrm << "GL_TEXTURE_BUFFER​"; break;
#endif
#ifdef GL_EXT_texture_array
        case GL_TEXTURE_1D_ARRAY_EXT: resStrm << "GL_TEXTURE_1D_ARRAY"; break;
        case GL_TEXTURE_2D_ARRAY_EXT: resStrm << "GL_TEXTURE_2D_ARRAY"; break;
#endif
#ifdef GL_ARB_texture_cube_map_array
        case GL_TEXTURE_CUBE_MAP_ARRAY_ARB: resStrm << "GL_TEXTURE_CUBE_MAP_ARRAY"; break;
#endif
#ifdef GL_ARB_texture_multisample
        case GL_TEXTURE_2D_MULTISAMPLE: resStrm << "GL_TEXTURE_2D_MULTISAMPLE"; break;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY: resStrm << "GL_TEXTURE_2D_MULTISAMPLE_ARRAY​"; break;
#endif
        default: std::cerr << "Texture Type with number "<< iter->second.type << " on stage "<< iter->first << " is not supported "  << std::endl; break;
    }

    resStrm  << "\nShadowTexture: " <<  iter->second.shadow;
  }

  if (!vertexTemplateFile.isEmpty())
    resStrm << "\nshaderDesc.vertexTemplateFile: " << vertexTemplateFile;

  if (!tessControlTemplateFile.isEmpty())
    resStrm << "\nshaderDesc.tessControlTemplateFile: " << tessControlTemplateFile;

  if (!tessEvaluationTemplateFile.isEmpty())
    resStrm << "\nshaderDesc.tessEvaluationTemplateFile: " << tessEvaluationTemplateFile;

  if (!geometryTemplateFile.isEmpty())
    resStrm << "\nshaderDesc.geometryTemplateFile: " << geometryTemplateFile;

  if (!fragmentTemplateFile.isEmpty())
    resStrm << "\nshaderDesc.fragmentTemplateFile: " << fragmentTemplateFile;

  return res;
}


} // namespace ACG
//=============================================================================
