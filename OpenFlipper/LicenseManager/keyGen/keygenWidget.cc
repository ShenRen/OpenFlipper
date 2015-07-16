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



#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

#include <QMessageBox>

#include "keygenWidget.hh"
#include <iostream>

#include "salt.hh"

KeyGen::KeyGen(QString n, QString cHash, QString pHash, QString cpHash, QString prHash, QStringList mHashes, QString request)
{
	name = n;
	coreHash = cHash;
	pluginHash = pHash;
	cpuHash = cpHash;
	productHash = prHash;
	macHashes = mHashes;
	requestSig = request;
}

QString KeyGen::computeSignature(const bool _utf8 ) const {
    // Get the salts
    QString saltPre;
    ADD_SALT_PRE(saltPre);
    QString saltPost;
    ADD_SALT_POST(saltPost);

    QString keyRequest = saltPre + name + coreHash + pluginHash + cpuHash
            + productHash + macHashes.join("") + saltPost;

    QString requestSigCheck;

    if ( _utf8 )
      requestSigCheck =
          QCryptographicHash::hash(keyRequest.toUtf8(),
              QCryptographicHash::Sha1).toHex();
    else
      requestSigCheck =
        QCryptographicHash::hash(keyRequest.toLatin1(),
            QCryptographicHash::Sha1).toHex();

    return requestSigCheck;
}

KeyGen::ValidationResult KeyGen::isValid() const
{
  if (requestSig == computeSignature(true))
  {
    return UTF8;
  }
  else if(requestSig == computeSignature(false))
  {
    return LATIN1;
  }
  return INVALID;
}

QString KeyGen::Generate(QString expiryDate) const
{
	// Get the salts
	QString saltPre;
	ADD_SALT_PRE(saltPre);
	QString saltPost;
	ADD_SALT_POST(saltPost);

	KeyGen::ValidationResult valid = isValid();

	if ( !valid ){
		return "ERROR";
	}
	else{
		QString license_ = "";

		// Add basic hashes
		license_ += expiryDate + "\n";
		license_ += name + "\n";
		license_ += coreHash + "\n";
		license_ += pluginHash + "\n";
		license_ += cpuHash + "\n";
		license_ += productHash + "\n";
		license_ += macHashes.join("\n") + "\n";

		QString licenseTmp = saltPre + expiryDate + name + coreHash + pluginHash + cpuHash + productHash + macHashes.join("") +  saltPost;
		QString licenseHash;
		if (valid == UTF8)
		  licenseHash = QCryptographicHash::hash ( licenseTmp.toUtf8()  , QCryptographicHash::Sha1 ).toHex();
		else
		  licenseHash = QCryptographicHash::hash ( licenseTmp.toLatin1()  , QCryptographicHash::Sha1 ).toHex();
		// Prepend signature
		license_ = licenseHash + "\n" + license_;
		return license_;
	}
}

QString KeyGen::filterString(QString in) {
    const QRegExp validChar("[a-f0-9]");
    QString out; out.reserve(in.size());
    for (QString::iterator it = in.begin(), it_end = in.end(); it != it_end; ++it) {
        if (validChar.exactMatch(*it))
            out.append(*it);
    }
    return out;
}

std::vector<KeyGen> KeyGen::CreateFromMessyString(QString info)
{
	const QString dirt = "[\\s;>]*";
	const QRegExp rx("\\b([\\w-]+)" + dirt + "((?:(?:[a-f0-9]" + dirt + "){40}){6,})\\b");
	const QRegExp partRe("((?:[a-f0-9]" + dirt + "){40})");

    std::vector<KeyGen> R;
	int pos = 0;
	while ((pos = rx.indexIn(info, pos)) != -1) {
	    QString hashesStr = rx.cap(2);
        QStringList hashes;
        int hashPos = 0;
	    while ((hashPos = partRe.indexIn(hashesStr, hashPos)) != -1) {
	        hashes.append(filterString(partRe.cap(1)));
	        hashPos += partRe.matchedLength();
	    }

	    QStringList macList;
	    std::copy(hashes.begin() + 4, hashes.end() - 1, std::back_inserter(macList));

		KeyGen K(rx.cap(1),
		        hashes[0],
		        hashes[1],
		        hashes[2],
		        hashes[3],
		        macList,
		        hashes[hashes.count()-1]);
		R.push_back(K);
		pos += rx.matchedLength();
	}

	return R;
}

KeyGenWidget::KeyGenWidget(QMainWindow *parent)
    : QMainWindow(parent)
{
  setupUi(this);
  connect(generateAllButton,SIGNAL(clicked()),this,SLOT(slotGenerateAllButton()));
  connect(generateLocalButton,SIGNAL(clicked()),this,SLOT(slotGenerateButton()));
  connect(keyList->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this, SLOT(handleSelectionChanged(QItemSelection)));
  
  connect(splitButton,SIGNAL(clicked()),this,SLOT(slotSplit()));
  
  connect(requestData,SIGNAL(textChanged()),this,SLOT(slotAnalyze()));
  
  // connect spinboxes forexpiry date
  connect(days  ,SIGNAL(valueChanged(int)),this,SLOT(slotDate()));
  connect(months,SIGNAL(valueChanged(int)),this,SLOT(slotDate()));
  connect(years ,SIGNAL(valueChanged(int)),this,SLOT(slotDate()));
  
  /*
   * Mangle Tab
   */
  connect(mangle_pb, SIGNAL(clicked()), this, SLOT(slotMangle()));

  // Automatically set expire date to current date
  // For security reasons no default span is set here!
  expires->setDate( QDate::currentDate());
  
  generateLocalButton->setVisible(false);
  generateAllButton->setVisible(false);
}

void KeyGenWidget::slotMangle() {
    const QString hardwareHash_raw = hardwareHashDump_te->toPlainText();
    const QString pluginHashes_raw = pluginHashDump_te->toPlainText();

    const std::vector<KeyGen> hardwareKeygens = KeyGen::CreateFromMessyString(hardwareHash_raw);
    if (hardwareKeygens.empty()) {
        QMessageBox::critical(this, tr("Unable to Mangle"), tr("No valid request found in hardware textbox."));
        return;
    }
    KeyGen hardwareKeygen = hardwareKeygens.front();

    std::vector<KeyGen> pluginKeygens = KeyGen::CreateFromMessyString(pluginHashes_raw);
    if (pluginKeygens.empty()) {
        QMessageBox::critical(this, tr("Unable to Mangle"), tr("No valid request found in plugins textbox."));
        return;
    }

    QString generatedRequest;
    for (std::vector<KeyGen>::iterator it = pluginKeygens.begin(), it_end = pluginKeygens.end();
            it != it_end; ++it) {

        it->copyHardwareHashesFrom(hardwareKeygen);

        generatedRequest += it->generateRequest();
    }

    requestData->setPlainText(generatedRequest);
}

void KeyGenWidget::slotDate() {
  QDate today = QDate::currentDate();
  today = today.addDays(days->value());
  today = today.addMonths(months->value());
  today = today.addYears(years->value());
  
  expires->setDate(today);
}

void KeyGenWidget::slotAnalyze() {
	QString inputData = requestData->toPlainText();
	keygens_ = KeyGen::CreateFromMessyString(inputData);

	keyList->clear();
	for (std::vector<KeyGen>::const_iterator it = keygens_.begin(), it_end = keygens_.end();
	        it != it_end; ++it) {
        QListWidgetItem *newItem = new QListWidgetItem( keyList);
        newItem->setText(it->name);
        newItem->setHidden(false);
        KeyGen::ValidationResult r = it->isValid();
        if (!r)
          newItem->setTextColor(QColor(255, 0, 0));
        else if (r == KeyGen::LATIN1)
          newItem->setTextColor(QColor(128, 128, 0));
	}

	generateLocalButton->setVisible(false);
	generateAllButton->setVisible(keygens_.size());
}

void KeyGenWidget::slotSplit() {
  // Get request data
  QString inputData = requestData->toPlainText();
  
  // Split with ;
  QStringList data = inputData.split(";",QString::SkipEmptyParts);
  
  QString newText = data.join("\n");
  
  requestData->setText(newText);
  
}

void KeyGenWidget::handleSelectionChanged(const QItemSelection& selection){
	generateLocalButton->setVisible(false);
	if(keyList->selectionModel()->selectedIndexes().count())
	{
		int i = keyList->selectionModel()->selectedIndexes()[0].row();
		setKeyGen(&keygens_[i]);
		generateLocalButton->setVisible(true);
		generateAllButton->setVisible(true);

		KeyGen::ValidationResult valid = keygens_[i].isValid();
		if (valid == KeyGen::INVALID)
		  lbWarning->setText("ERROR: Signature does not match.\nCannot generate key");
		else if (valid == KeyGen::LATIN1)
		  lbWarning->setText("WARNING: Request uses old Ascii format.\nKey will be generated with Ascii encoding.");
		else
		  lbWarning->setText("");
	}
}

KeyGenWidget::~KeyGenWidget() {

}

void KeyGenWidget::toFile(const KeyGen* gen)
{
	QString licenseFileName_ = gen->name;
  std::cerr << "Writing License file to output : " << licenseFileName_.toStdString() << std::endl;
  QFile outFile(licenseFileName_ + ".lic");

  if (!outFile.open(QIODevice::WriteOnly|QIODevice::Text)) {
	  QMessageBox::critical(this,tr("Unable to open file"),tr("Unable to Open output File"));
	  return;
  }

  QTextStream output(&outFile);
  output << gen->Generate(expires->date().toString(Qt::ISODate));
  outFile.close();
}

void KeyGenWidget::setKeyGen(const KeyGen* gen) {
	fileNameBox->setText(gen->name);
	coreHashBox->setText(gen->coreHash);
	pluginHashBox->setText(gen->pluginHash);
	cpuHashBox->setText(gen->cpuHash);
	productIDBox->setText(gen->productHash);
	macHashBox->setText(gen->macHashes.join("\n"));
	signatureBox->setText(gen->requestSig);
	generateLocalButton->setEnabled(gen->isValid());
}

void KeyGenWidget::slotGenerateButton() {
	if(keyList->selectionModel()->selectedIndexes().count())
	{
		int i = keyList->selectionModel()->selectedIndexes()[0].row();
		toFile(&keygens_[i]);
	}
}

void KeyGenWidget::slotGenerateAllButton() {
  for(unsigned int i = 0; i < keygens_.size(); i++)
	  toFile(&keygens_[i]);
}
