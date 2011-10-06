/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-9 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#ifndef __DROWAUDIO_MUSICLIBRARYTABLE_H__
#define __DROWAUDIO_MUSICLIBRARYTABLE_H__

#include "../audio/dRowAudio_AudioUtility.h"
#include "../utility/dRowAudio_ITunesLibrary.h"
#include "../utility/dRowAudio_Comparators.h"

//==============================================================================
/**
    This class shows how to implement a TableListBoxModel to show in a TableListBox.
*/
class MusicLibraryTable	: public Component,
                          public TableListBoxModel,
						  public Timer,
						  public ITunesLibrary::Listener
{
public:
    //==============================================================================
    MusicLibraryTable();

    ~MusicLibraryTable();

    //==============================================================================
	void setLibraryToUse (ITunesLibrary *library);

	void libraryChanged (ITunesLibrary *library);
	
	void libraryUpdated (ITunesLibrary *library);
	
	void libraryFinished (ITunesLibrary *library);
	
    //==============================================================================
    // This is overloaded from TableListBoxModel, and must return the total number of rows in our table
    int getNumRows();

    // This is overloaded from TableListBoxModel, and should fill in the background of the whole row
    void paintRowBackground (Graphics& g, int rowNumber, int width, int height, bool rowIsSelected);

    // This is overloaded from TableListBoxModel, and must paint any cells that aren't using custom
    // components.
    void paintCell (Graphics& g,
                    int rowNumber,
                    int columnId,
                    int width, int height,
                    bool rowIsSelected);

    // This is overloaded from TableListBoxModel, and tells us that the user has clicked a table header
    // to change the sort order.
    void sortOrderChanged (int newSortColumnId, const bool isForwards);

    // This is overloaded from TableListBoxModel, and should choose the best width for the specified
    // column.
    int getColumnAutoSizeWidth (int columnId);

	// Call this to sort the table displaying only the items matched
	void setFilterText (String filterText);
		
	//	Returns the table list box component.
	TableListBox* getTableListBox()	{	return table;	};
	
    //==============================================================================
    void resized();

	void focusOfChildComponentChanged (FocusChangeType cause); 

	void timerCallback();
	
	var getDragSourceDescription (const SparseSet<int>& currentlySelectedRows);
	
    //==============================================================================

private:
    //==============================================================================
    Font font;	
//	ScopedPointer<ITunesLibraryParser> parser;	
	ITunesLibrary *currentLibrary;

    ScopedPointer<ValueTree> demoData;   // This is the XML document loaded from the embedded file "demo table data.xml"
    ValueTree columnList; // A pointer to the sub-node of demoData that contains the list of columns
    ValueTree dataList;   // A pointer to the sub-node of demoData that contains the list of data rows
    int numRows;            // The number of rows of data we've got

    TableListBox* table;    // the table component itself
	
	ScopedPointer<ValueTree> filteredDataList;   // A pointer to the sub-node of demoData that contains the list of data rows
    int filteredNumRows;            // The number of rows of data we've got
	bool finishedLoading;
	
    //==============================================================================
	void setUpColumns (ValueTree &elementToSetUp)
	{
		for (int i = 1; i < Columns::numColumns; i++)
		{
			if (i != Columns::Score)
			{
				ValueTree tempElement("COLUMN");
				tempElement.setProperty("columnId", i, 0);
				tempElement.setProperty("name", Columns::columnNames[i].toString(), 0);
				tempElement.setProperty("width", Columns::columnWidths[i], 0);
				
				elementToSetUp.addChild(tempElement, -1, 0);
			}
		}
	}
	
	void loadData()
	{
        demoData = new ValueTree("DEMO_TABLE_DATA");
		
		demoData->addChild(ValueTree("COLUMNS"), -1, 0);
		columnList = demoData->getChildWithName("COLUMNS");
		setUpColumns(columnList);
				
		numRows = dataList.getNumChildren();
	}
};

#endif // __DROWAUDIO_MUSICLIBRARYTABLE_H__