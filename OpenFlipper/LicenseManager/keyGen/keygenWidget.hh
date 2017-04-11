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


#ifndef KEYGENWINDOW_HH
#define KEYGENWINDOW_HH

#include <ui_keygen.hh>

class KeyGen
{
	public:
		QString name;
		QString coreHash;
		QString pluginHash;
		QString cpuHash;
		QString productHash;
		QStringList macHashes;
		QString requestSig;
	public:
		KeyGen(QString n, QString cHash, QString pHash, QString cpHash, QString prHash, QStringList mHashes, QString request);
		//returns string containing the key
		QString Generate(QString expiryDate) const;
		//finds all occurrences of info in messy string
		QString computeSignature(bool _utf8 = true) const;
		enum ValidationResult
		{
		  INVALID = 0,
		  UTF8 = 1,
		  LATIN1 = 2
		};
		// returns if the result is valid. INVALID, if invalid, UTF8 if the signature is in new utf8 encoding, latin1 if the signature is in deprecated latin1 encoding
		ValidationResult isValid() const;

		void copyHardwareHashesFrom(const KeyGen &rhs) {
		    cpuHash = rhs.cpuHash;
		    macHashes = rhs.macHashes;
		}

		QString generateRequest() {
		    return QString("%1\n%2\n%3\n%4\n%5\n%6\n%7\n\n")
		            .arg(name)
		            .arg(coreHash)
		            .arg(pluginHash)
		            .arg(cpuHash)
		            .arg(productHash)
		            .arg(macHashes.join("\n"))
		            .arg(computeSignature());
		}

		static std::vector<KeyGen> CreateFromMessyString(QString info);

		static QString filterString(QString in);
};


class KeyGenWidget : public QMainWindow, public Ui::keyWindow
{
  Q_OBJECT

public:
  explicit KeyGenWidget(QMainWindow *parent);
  virtual ~KeyGenWidget();

public slots:
  void slotGenerateAllButton();
  void slotGenerateButton();
  
  // slot taking license duration and convert to expiration date
  void slotDate();
  
  // Analyzes the current request
  void slotAnalyze();
  
  // Split Code based on ;; for broken windows requests
  void slotSplit();
  
  void handleSelectionChanged(const QItemSelection& selection);

  void slotMangle();

private:

  void toFile(const KeyGen* gen);
  void setKeyGen(const KeyGen* gen);

  std::vector<KeyGen> keygens_;

};

 #endif // KEYGENWINDOW_HH
