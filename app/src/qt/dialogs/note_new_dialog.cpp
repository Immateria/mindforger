/*
 note_new_dialog.cpp     MindForger thinking notebook

 Copyright (C) 2016-2024 Martin Dvorak <martin.dvorak@mindforger.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include "note_new_dialog.h"

using namespace std;

namespace m8r {

/*
 * General tab
 */

NoteNewDialog::GeneralTab::GeneralTab(Ontology& ontology, QWidget *parent)
    : QWidget(parent), ontology(ontology)
{
    QGroupBox* basicGroup = new QGroupBox{tr("Basic"), this};

    nameLabel = new QLabel(tr("Name")+":", this),
    nameEdit = new QLineEdit(tr("Note"), this);

    emojisButton = new QPushButton(tr("&Emojis"), this);

    // moving edit tags to this position changes TAB ORDER ~ it's selected as 2nd field
    editTagsGroup = new EditTagsPanel{MfWidgetMode::CREATE_MODE, ontology, this};
    editTagsGroup->refreshOntologyTags();

    positionLabel = new QLabel(tr("Position")+":", this);
    positionCombo = new QComboBox(this);

    viewEditLabel = new QLabel(tr("Edit or view after creation")+":", this);
    viewEditCombo = new QComboBox(this);

    stencilLabel = new QLabel(tr("Stencil")+":", this);
    stencilCombo = new QComboBox(this);

    typeLabel = new QLabel(tr("Type")+":", this);
    typeCombo = new QComboBox(this);

    progressLabel = new QLabel{tr("Progress")+": %", this};
    progressSpin = new QSpinBox{this};
    progressSpin->setMinimum(0);
    progressSpin->setMaximum(100);

    // assembly
    QVBoxLayout* basicLayout = new QVBoxLayout{this};
    basicLayout->addWidget(nameLabel);

    QWidget* eji = new QWidget{this};
    QHBoxLayout* nameLayout = new QHBoxLayout{eji};
    nameLayout->addWidget(nameEdit);
    nameLayout->addWidget(emojisButton);
    basicLayout->addWidget(eji);

    basicLayout->addWidget(positionLabel);
    basicLayout->addWidget(positionCombo);
    basicLayout->addWidget(viewEditLabel);
    basicLayout->addWidget(viewEditCombo);
    basicLayout->addWidget(stencilLabel);
    basicLayout->addWidget(stencilCombo);
    basicLayout->addWidget(typeLabel);
    basicLayout->addWidget(typeCombo);
    basicLayout->addWidget(progressLabel);
    basicLayout->addWidget(progressSpin);
    basicGroup->setLayout(basicLayout);

    QVBoxLayout* boxesLayout = new QVBoxLayout{this};
    boxesLayout->addWidget(basicGroup);
    boxesLayout->addWidget(editTagsGroup);
    setLayout(boxesLayout);
}

NoteNewDialog::GeneralTab::~GeneralTab()
{
    delete nameLabel;
    delete nameEdit;
    delete typeLabel;
    delete typeCombo;
    delete positionLabel;
    delete positionCombo;
    delete viewEditLabel;
    delete viewEditCombo;
    delete progressLabel;
    delete progressSpin;
    delete stencilLabel;
    delete stencilCombo;
}

void NoteNewDialog::GeneralTab::clean()
{
    nameEdit->setText(tr("Note"));
    nameEdit->selectAll();
    nameEdit->setFocus();
    stencilCombo->clear();
    editTagsGroup->clearTagList();
}

void NoteNewDialog::GeneralTab::showFacet(Repository::RepositoryType repositoryType)
{
    bool visibility =
        Repository::RepositoryType::MINDFORGER == repositoryType
        ? true
        : false;

    editTagsGroup->setVisible(visibility);
    progressLabel->setVisible(visibility);
    progressSpin->setVisible(visibility);
    stencilLabel->setVisible(visibility);
    stencilCombo->setVisible(visibility);
    typeLabel->setVisible(visibility);
    typeCombo->setVisible(visibility);
}

/*
 * Advanced tab.
 */

NoteNewDialog::AdvancedTab::AdvancedTab(QWidget* parent)
    : QWidget(parent)
{
    fileLabel = new QLabel{tr("File")+":", this};
    fileLine = new QLineEdit{this};
    fileLine->setEnabled(false);

    QGroupBox* locationGroup = new QGroupBox{tr("Location"), this};
    QVBoxLayout* locationLayout = new QVBoxLayout{this};
    locationLayout->addWidget(fileLabel);
    locationLayout->addWidget(fileLine);
    locationGroup->setLayout(locationLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(locationGroup);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

void NoteNewDialog::AdvancedTab::refreshLocation(const QString& path)
{
    fileLine->setText(path);
}

NoteNewDialog::AdvancedTab::~AdvancedTab()
{
}

/*
 * Dialog.
 */

NoteNewDialog::NoteNewDialog(
        // IMPROVE Does model belong to View? Set values from presenter & keep model classes in presenter only?
        Ontology& ontology,
        QWidget* parent)
    : QDialog(parent), ontology(ontology)
{
    tabWidget = new QTabWidget;

    generalTab = new GeneralTab{ontology, this};
    if(ontology.getNoteTypes().size()) {
        QComboBox* combo=generalTab->getTypeCombo();
        for(const NoteType* t:ontology.getNoteTypes().values()) {
            combo->addItem(QString::fromStdString(t->getName()), QVariant::fromValue<const NoteType*>(t));
        }
        combo->setCurrentText(QString::fromStdString(ontology.getDefaultNoteType()->getName()));
    }
    QComboBox* combo=generalTab->getPositionCombo();
    // IMPROVE i18n - text is localized, QVariant keeps a constant (e.g. enum)
    combo->addItem("Above selected Note");
    combo->addItem("Below selection Note");
    combo=generalTab->getViewEditCombo();
    // IMPROVE i18n - text is localized, QVariant keeps a constant (e.g. enum)
    combo->addItem("Edit");
    combo->addItem("View");
    // stencils may be added/changed/removed - refreshed each time dialog is shown
    tabWidget->addTab(generalTab, tr("General"));

    advancedTab = new AdvancedTab{this};
    tabWidget->addTab(advancedTab, tr("Advanced"));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    // signals
    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout{this};
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("New Note"));
    resize(fontMetrics().averageCharWidth()*55, fontMetrics().height()*30);
    setModal(true);
}

NoteNewDialog::~NoteNewDialog()
{
    if(generalTab) delete generalTab;
    if(advancedTab) delete advancedTab;
}

QString NoteNewDialog::getNoteName() const
{
    return generalTab->getNameEdit()->text();
}

Stencil* NoteNewDialog::getStencil() const
{
    return generalTab->getStencilCombo()->itemData(generalTab->getStencilCombo()->currentIndex()).value<Stencil*>();
}

const NoteType* NoteNewDialog::getNoteType() const
{
    return (const NoteType*)(generalTab->getTypeCombo()->itemData(generalTab->getTypeCombo()->currentIndex(), Qt::UserRole).value<const NoteType*>());
}

const std::vector<const Tag*>& NoteNewDialog::getTags() const
{
    return generalTab->getTags();
}

int NoteNewDialog::getProgress() const
{
    return generalTab->getProgressSpin()->value();
}

QPushButton* NoteNewDialog::getEmojisButton() const
{
    return generalTab->getEmojisButton();
}

void NoteNewDialog::show(
    const QString& path,
    vector<Stencil*>& stencils,
    Repository::RepositoryType repositoryType
) {
    // try to remember previous stencil
    QString selectedStencil = generalTab->getStencilCombo()->currentText();

    generalTab->clean();

    // IMPROVE re-load only if stencils are dirty
    if(stencils.size()) {
        QComboBox* combo=generalTab->getStencilCombo();
        combo->setEnabled(true);
        combo->addItem("", QVariant::fromValue<const Stencil*>(nullptr));
        for(Stencil* t:stencils) {
            if(t->getType() == ResourceType::NOTE) {
                combo->addItem(QString::fromStdString(t->getName()), QVariant::fromValue<Stencil*>(t));
            }
        }
        combo->setCurrentText(selectedStencil);
    } else {
        generalTab->getStencilCombo()->clear();
        generalTab->getStencilCombo()->setEnabled(false);
    }

    advancedTab->refreshLocation(path);

    // widgets visibility: Markdown vs. MindForger (repository) mode
    generalTab->showFacet(repositoryType);

    QDialog::show();
}

bool NoteNewDialog::isPositionBelow() const {
    return generalTab->isPositionBelow();
}

bool NoteNewDialog::isOpenInEditor() const {
    return generalTab->isOpenInEditor();
}

}
