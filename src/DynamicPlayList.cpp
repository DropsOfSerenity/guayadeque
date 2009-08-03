// -------------------------------------------------------------------------------- //
//	Copyright (C) 2008-2009 J.Rios
//	anonbeat@gmail.com
//
//    This Program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2, or (at your option)
//    any later version.
//
//    This Program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; see the file LICENSE.  If not, write to
//    the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
//    http://www.gnu.org/copyleft/gpl.html
//
// -------------------------------------------------------------------------------- //
#include "DynamicPlayList.h"

#include "Images.h"
#include "Utils.h"

WX_DEFINE_OBJARRAY(guFilterItemArray);


wxString m_FilterFieldChoices[] = {
    _("Title"), _("Artist"), _("Album"), _("Genre"), _( "Label" ),
    _("Path"), _("Year"), _("Rating"), _("Length"),
    _("Play Count"), _("Last Play Time"), _("Added Date")
};

wxString m_FilterTextOptionChoices[] = { _("Contains"), _("Does not contains"), _("Equals"),
                                           _("Begins with"), _("Ends with") };

wxString m_FilterNumberOptionChoices[] = { _( "equals" ), _( "at least" ), _( "at most" ) };

wxString m_FilterYearOptionChoices[] = { _( "in" ), _( "after" ), _( "before" ) };

wxString m_FilterDateOptionChoices[] = { _( "in the last" ), _( "before the last" ) };

wxString m_LimitChoices[] = { _("Tracks"), _("Minutes"), wxT("MB"), wxT("GB") };

wxString m_SortChoices[] = {
    _("Title"), _("Artist"), _( "Album" ), _( "Genre" ), _( "Label" ),
    _( "Year" ), _( "Rating" ), _( "Length" ),
    _( "Play Count" ), _( "Last Play Time" ), _( "Added Time" )
};

wxString m_FilterDateOption2Choices[] = { _( "minutes" ), _( "hours" ), _( "days" ), _( "weeks" ), _( "months" ) };

// -------------------------------------------------------------------------------- //
// guFilterItem
// -------------------------------------------------------------------------------- //
void guFilterItem::Set( int type, int option, const wxString &text )
{
    m_Type = type;
    m_Option = option;
    m_Text = text;
    m_Number = 0;
    SetFilterLabel();
}

// -------------------------------------------------------------------------------- //
void guFilterItem::Set( int type, int option, int number, int option2 )
{
    m_Type = type;
    m_Option = option;
    m_Number = number;
    m_Option2 = option2;
    m_Text = wxEmptyString;
    SetFilterLabel();
}

// -------------------------------------------------------------------------------- //
void guFilterItem::SetFilterLabel( void )
{
    m_Label = m_FilterFieldChoices[ m_Type ] + wxT( " " );
    switch( m_Type )
    {
        case guDYNAMIC_FILTER_TYPE_TITLE : // String
        case guDYNAMIC_FILTER_TYPE_ARTIST :
        case guDYNAMIC_FILTER_TYPE_ALBUM :
        case guDYNAMIC_FILTER_TYPE_GENRE :
        case guDYNAMIC_FILTER_TYPE_LABEL :
        case guDYNAMIC_FILTER_TYPE_PATH :
        {
            m_Label += m_FilterTextOptionChoices[ m_Option ] + wxT( " " );
            m_Label += m_Text;
            break;
        }

        case guDYNAMIC_FILTER_TYPE_YEAR : // Year
        {
            m_Label += m_FilterYearOptionChoices[ m_Option ];
            m_Label += wxString::Format( wxT( " %u" ), m_Number );
            break;
        }

        case guDYNAMIC_FILTER_TYPE_RATING : // Numbers
        case guDYNAMIC_FILTER_TYPE_PLAYCOUNT :
        {
            m_Label += m_FilterNumberOptionChoices[ m_Option ];
            m_Label += wxString::Format( wxT( " %u" ), m_Number );
            break;
        }

        case guDYNAMIC_FILTER_TYPE_LENGTH : // Time
        {
            m_Label += m_FilterNumberOptionChoices[ m_Option ] + wxT( " " );
            m_Label += LenToString( m_Number );
            break;
        }

        case guDYNAMIC_FILTER_TYPE_LASTPLAY :
        case guDYNAMIC_FILTER_TYPE_ADDEDDATE :
        {
            m_Label += m_FilterDateOptionChoices[ m_Option ];
            m_Label += wxString::Format( wxT( " %u " ), m_Number );
            m_Label += m_FilterDateOption2Choices[ m_Option2 ];
        }
    }
}

// -------------------------------------------------------------------------------- //
// guDynPlayLIstEditor
// -------------------------------------------------------------------------------- //
guDynPlayListEditor::guDynPlayListEditor( wxWindow * parent, guDynPlayList * playlist ) :
  wxDialog( parent, wxID_ANY, _( "Dynamic Playlist Editor" ), wxDefaultPosition, wxSize( 600,400 ), wxDEFAULT_DIALOG_STYLE )
{
	int index;
	int count;
	m_PlayList = playlist;
	m_Filters = &m_PlayList->m_Filters;
	m_CurFilter = wxNOT_FOUND;
	m_HasChanged = false;

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer * MainSizer;
	MainSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* CurFiltersSizer;
	CurFiltersSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _( " Current Filters " ) ), wxVERTICAL );

	m_FiltersListBox = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_FiltersListBox->SetMinSize( wxSize( -1,80 ) );

	index;
	count = m_Filters->Count();
	//guLogMessage( wxT( "Found %u filters" ), count );
	for( index = 0; index < count; index++ )
	{
	    m_FiltersListBox->Append( ( * m_Filters )[ index ].GetLabel() );
	    //guLogMessage( wxT( "Filter %u : %s" ), index, ( * m_Filters )[ index ].GetLabel().c_str() );
	}

	CurFiltersSizer->Add( m_FiltersListBox, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_FilterEditSizer = new wxBoxSizer( wxHORIZONTAL );

	int ChoiceCount = sizeof( m_FilterFieldChoices ) / sizeof( wxString );
	m_FilterFieldChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ChoiceCount, m_FilterFieldChoices, 0 );
	m_FilterFieldChoice->SetSelection( 0 );
	m_FilterEditSizer->Add( m_FilterFieldChoice, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	ChoiceCount = sizeof( m_FilterTextOptionChoices ) / sizeof( wxString );
	m_FilterOptionChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ChoiceCount, m_FilterTextOptionChoices, 0 );
	m_FilterOptionChoice->SetSelection( 0 );
	m_FilterEditSizer->Add( m_FilterOptionChoice, 0, wxBOTTOM|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_FilterText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_FilterEditSizer->Add( m_FilterText, 1, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT, 5 );

	m_FilterRating = new guRating( this, GURATING_STYLE_BIG );
	m_FilterRating->Show( false );
	m_FilterEditSizer->Add( m_FilterRating, 1, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT, 5 );

	ChoiceCount = sizeof( m_FilterDateOption2Choices ) / sizeof( wxString );
	m_FilterDateOption2Choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ChoiceCount, m_FilterDateOption2Choices, 0 );
	m_FilterDateOption2Choice->SetSelection( 0 );
	m_FilterDateOption2Choice->Show( false );
	m_FilterEditSizer->Add( m_FilterDateOption2Choice, 0, wxBOTTOM|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );

    m_LengthHours = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 9999 );
    m_LengthHours->Show( false );
	m_FilterEditSizer->Add( m_LengthHours, 1, wxBOTTOM|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );

    m_LengthSeparator1 = new wxStaticText( this, wxID_ANY, wxT( ":" ) );
    m_LengthSeparator1->Show( false );
	m_FilterEditSizer->Add( m_LengthSeparator1, 0, wxBOTTOM|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );

    m_LengthMinutes = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 59 );
    m_LengthMinutes->Show( false );
	m_FilterEditSizer->Add( m_LengthMinutes, 1, wxBOTTOM|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );

    m_LengthSeparator2 = new wxStaticText( this, wxID_ANY, wxT( ":" ) );
    m_LengthSeparator2->Show( false );
	m_FilterEditSizer->Add( m_LengthSeparator2, 0, wxBOTTOM|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );

    m_LengthSeconds = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 59 );
    m_LengthSeconds->Show( false );
	m_FilterEditSizer->Add( m_LengthSeconds, 1, wxBOTTOM|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_FilterAdd = new wxBitmapButton( this, wxID_ANY, guImage( guIMAGE_INDEX_tiny_add ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_FilterAdd->Enable( false );
	m_FilterEditSizer->Add( m_FilterAdd, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM, 5 );

	m_FilterDel = new wxBitmapButton( this, wxID_ANY, guImage( guIMAGE_INDEX_tiny_del ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_FilterDel->Enable( false );
	m_FilterEditSizer->Add( m_FilterDel, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT, 5 );

	m_FilterAccept = new wxBitmapButton( this, wxID_ANY, guImage( guIMAGE_INDEX_tiny_accept ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_FilterAccept->Enable( false );
	m_FilterEditSizer->Add( m_FilterAccept, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	CurFiltersSizer->Add( m_FilterEditSizer, 0, wxEXPAND, 5 );

	MainSizer->Add( CurFiltersSizer, 1, wxEXPAND|wxALL, 5 );

	wxStaticBoxSizer* ResultSizer;
	ResultSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _(" Result ") ), wxVERTICAL );

	wxBoxSizer* LimitSizer;
	LimitSizer = new wxBoxSizer( wxHORIZONTAL );

	m_LimitCheckBox = new wxCheckBox( this, wxID_ANY, _("Limit To"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LimitCheckBox->SetValue( m_PlayList->m_Limited );

	LimitSizer->Add( m_LimitCheckBox, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_LimitSpinCtrl = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 2147483647, 0 );
	m_LimitSpinCtrl->SetValue( m_PlayList->m_LimitValue );
	m_LimitSpinCtrl->Enable( m_LimitCheckBox->IsChecked() );

	LimitSizer->Add( m_LimitSpinCtrl, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	int m_LimitChoiceNChoices = sizeof( m_LimitChoices ) / sizeof( wxString );
	m_LimitChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_LimitChoiceNChoices, m_LimitChoices, 0 );
	m_LimitChoice->SetSelection( m_PlayList->m_LimitType );
	m_LimitChoice->Enable( m_LimitCheckBox->IsChecked() );

	LimitSizer->Add( m_LimitChoice, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	ResultSizer->Add( LimitSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* SortSizer;
	SortSizer = new wxBoxSizer( wxHORIZONTAL );

	m_SortCheckBox = new wxCheckBox( this, wxID_ANY, _("Sort By"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SortCheckBox->SetValue( m_PlayList->m_Sorted );
	SortSizer->Add( m_SortCheckBox, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	ChoiceCount = sizeof( m_SortChoices ) / sizeof( wxString );
	m_SortChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ChoiceCount, m_SortChoices, 0 );
	m_SortChoice->SetSelection( m_PlayList->m_SortType );
	m_SortChoice->Enable( m_SortCheckBox->IsChecked() );

	SortSizer->Add( m_SortChoice, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	m_DescCheckBox = new wxCheckBox( this, wxID_ANY, _("Descending"), wxDefaultPosition, wxDefaultSize, 0 );
	m_DescCheckBox->SetValue( m_PlayList->m_SortDesc );
	m_DescCheckBox->Enable( m_SortCheckBox->IsChecked() );

	SortSizer->Add( m_DescCheckBox, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );


	SortSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_AddOnAnyCheckBox = new wxCheckBox( this, wxID_ANY, _("Add tracks on any criteria"), wxDefaultPosition, wxDefaultSize, 0 );
	m_AddOnAnyCheckBox->SetValue( m_PlayList->m_AnyOption );

	SortSizer->Add( m_AddOnAnyCheckBox, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	ResultSizer->Add( SortSizer, 1, wxEXPAND, 5 );

	MainSizer->Add( ResultSizer, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    wxStdDialogButtonSizer * ButtonsSizer;
    wxButton *  BtnCancel;

	ButtonsSizer = new wxStdDialogButtonSizer();
	m_BtnOk = new wxButton( this, wxID_OK );
	m_BtnOk->Enable( m_Filters->Count() );
	ButtonsSizer->AddButton( m_BtnOk );
	BtnCancel = new wxButton( this, wxID_CANCEL );
	ButtonsSizer->AddButton( BtnCancel );
	ButtonsSizer->Realize();
	MainSizer->Add( ButtonsSizer, 0, wxEXPAND, 5 );

	this->SetSizer( MainSizer );
	this->Layout();

	// Connect Events
	m_FiltersListBox->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( guDynPlayListEditor::OnFiltersSelected ), NULL, this );

	m_FilterFieldChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( guDynPlayListEditor::OnFilterFieldSelected ), NULL, this );
	m_FilterOptionChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( guDynPlayListEditor::OnFilterOptionSelected ), NULL, this );
	m_FilterText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( guDynPlayListEditor::OnFilterTextChanged ), NULL, this );
	m_FilterRating->Connect( guEVT_RATING_CHANGED, guRatingEventHandler( guDynPlayListEditor::OnRatingChanged ), NULL, this );
	m_FilterDateOption2Choice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( guDynPlayListEditor::OnFilterDateOption2Selected ), NULL, this );
	m_FilterAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( guDynPlayListEditor::OnFilterAddClicked ), NULL, this );
	m_FilterDel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( guDynPlayListEditor::OnFilterDelClicked ), NULL, this );
	m_FilterAccept->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( guDynPlayListEditor::OnFilterUpdateClicked ), NULL, this );
	m_LimitCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( guDynPlayListEditor::OnLImitChecked ), NULL, this );
	m_SortCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( guDynPlayListEditor::OnSortChecked ), NULL, this );

}

// -------------------------------------------------------------------------------- //
guDynPlayListEditor::~guDynPlayListEditor()
{
	m_FiltersListBox->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( guDynPlayListEditor::OnFiltersSelected ), NULL, this );

	m_FilterFieldChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( guDynPlayListEditor::OnFilterFieldSelected ), NULL, this );
	m_FilterOptionChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( guDynPlayListEditor::OnFilterOptionSelected ), NULL, this );
	m_FilterText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( guDynPlayListEditor::OnFilterTextChanged ), NULL, this );
	m_FilterRating->Disconnect( guEVT_RATING_CHANGED, guRatingEventHandler( guDynPlayListEditor::OnRatingChanged ), NULL, this );
	m_FilterDateOption2Choice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( guDynPlayListEditor::OnFilterDateOption2Selected ), NULL, this );
	m_FilterAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( guDynPlayListEditor::OnFilterAddClicked ), NULL, this );
	m_FilterDel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( guDynPlayListEditor::OnFilterDelClicked ), NULL, this );
	m_FilterAccept->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( guDynPlayListEditor::OnFilterUpdateClicked ), NULL, this );
	m_LimitCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( guDynPlayListEditor::OnLImitChecked ), NULL, this );
	m_SortCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( guDynPlayListEditor::OnSortChecked ), NULL, this );
}

// -------------------------------------------------------------------------------- //
void guDynPlayListEditor::FillPlayListEditData( void )
{
    m_PlayList->m_Limited = m_LimitCheckBox->IsChecked();
    m_PlayList->m_LimitValue = m_LimitSpinCtrl->GetValue();
    m_PlayList->m_LimitType = m_LimitChoice->GetSelection();
    m_PlayList->m_Sorted = m_SortCheckBox->IsChecked();
    m_PlayList->m_SortType = m_SortChoice->GetSelection();
    m_PlayList->m_SortDesc = m_DescCheckBox->IsChecked();
    m_PlayList->m_AnyOption = m_AddOnAnyCheckBox->IsChecked();
}

// -------------------------------------------------------------------------------- //
guFilterItem guDynPlayListEditor::GetFilterItem( void )
{
    guFilterItem FilterItem;
    int FilterType = m_FilterFieldChoice->GetSelection();
    if( FilterType < guDYNAMIC_FILTER_TYPE_YEAR )
    {
        FilterItem.Set( FilterType,
                    m_FilterOptionChoice->GetSelection(),
                    FilterItem.m_Text = m_FilterText->GetValue() );
    }
    else if( FilterType == guDYNAMIC_FILTER_TYPE_RATING )
    {
        FilterItem.Set( FilterType,
                        m_FilterOptionChoice->GetSelection(),
                        m_FilterRating->GetRating(),
                        0 );
    }
    else if( FilterType == guDYNAMIC_FILTER_TYPE_LENGTH )
    {
        unsigned long hour = m_LengthHours->GetValue();
        unsigned long min = m_LengthMinutes->GetValue();
        unsigned long sec = m_LengthSeconds->GetValue();
        FilterItem.Set( FilterType,
                        m_FilterOptionChoice->GetSelection(),
                        ( hour * 3600 ) + ( min * 60 ) + sec,
                        0 );

    }
    else if( FilterType > guDYNAMIC_FILTER_TYPE_PLAYCOUNT )
    {
        unsigned long value = 0;
        m_FilterText->GetValue().ToULong( &value );
        FilterItem.Set( FilterType,
                        m_FilterOptionChoice->GetSelection(),
                        value,
                        m_FilterDateOption2Choice->GetSelection() );
    }
    else
    {
        unsigned long value = 0;
        m_FilterText->GetValue().ToULong( &value );
        FilterItem.Set( FilterType,
                        m_FilterOptionChoice->GetSelection(),
                        value,
                        0 );
    }
    return FilterItem;
}

// -------------------------------------------------------------------------------- //
bool guDynPlayListEditor::FilterHasChanged( void )
{
    return m_HasChanged;
}

// -------------------------------------------------------------------------------- //
void guDynPlayListEditor::UpdateEditor( int FilterType )
{
    int index;
    int count;

    if( FilterType == guDYNAMIC_FILTER_TYPE_LENGTH )
    {
        //
        m_FilterAdd->Enable( true );
    }
    else
    {
        //
        m_FilterAdd->Enable( false );
    }

    if( FilterType == guDYNAMIC_FILTER_TYPE_RATING )
    {
        m_FilterText->Show( false );
        m_FilterRating->Show( true );
        m_FilterDateOption2Choice->Show( false );
        m_LengthHours->Show( false );
        m_LengthSeparator1->Show( false );
        m_LengthMinutes->Show( false );
        m_LengthSeparator2->Show( false );
        m_LengthSeconds->Show( false );
    }
    else if( FilterType == guDYNAMIC_FILTER_TYPE_LENGTH )
    {
        m_FilterText->Show( false );
        m_FilterRating->Show( false );
        m_FilterDateOption2Choice->Show( false );
        m_LengthHours->Show( true );
        m_LengthSeparator1->Show( true );
        m_LengthMinutes->Show( true );
        m_LengthSeparator2->Show( true );
        m_LengthSeconds->Show( true );

    }
    else if( FilterType > guDYNAMIC_FILTER_TYPE_PLAYCOUNT )
    {
        m_FilterText->Show( true );
        m_FilterRating->Show( false );
        m_FilterDateOption2Choice->Show( true );
        m_LengthHours->Show( false );
        m_LengthSeparator1->Show( false );
        m_LengthMinutes->Show( false );
        m_LengthSeparator2->Show( false );
        m_LengthSeconds->Show( false );
    }
    else
    {
        m_FilterText->Show( true );
        m_FilterRating->Show( false );
        m_FilterDateOption2Choice->Show( false );
        m_LengthHours->Show( false );
        m_LengthSeparator1->Show( false );
        m_LengthMinutes->Show( false );
        m_LengthSeparator2->Show( false );
        m_LengthSeconds->Show( false );
    }

    m_FilterOptionChoice->Clear();

    if( FilterType < guDYNAMIC_FILTER_TYPE_YEAR )
    {
        count = sizeof( m_FilterTextOptionChoices ) / sizeof( wxString );
        for( index = 0; index < count; index++ )
        {
            m_FilterOptionChoice->Append( m_FilterTextOptionChoices[ index ] );
        }
    }
    else if( FilterType == guDYNAMIC_FILTER_TYPE_YEAR )
    {
        count = sizeof( m_FilterYearOptionChoices ) / sizeof( wxString );
        for( index = 0; index < count; index++ )
        {
            m_FilterOptionChoice->Append( m_FilterYearOptionChoices[ index ] );
        }
    }
    else if( FilterType < guDYNAMIC_FILTER_TYPE_LASTPLAY )
    {
        count = sizeof( m_FilterNumberOptionChoices ) / sizeof( wxString );
        for( index = 0; index < count; index++ )
        {
            m_FilterOptionChoice->Append( m_FilterNumberOptionChoices[ index ] );
        }
    }
    else if( FilterType > guDYNAMIC_FILTER_TYPE_PLAYCOUNT )
    {
        count = sizeof( m_FilterDateOptionChoices ) / sizeof( wxString );
        for( index = 0; index < count; index++ )
        {
            m_FilterOptionChoice->Append( m_FilterDateOptionChoices[ index ] );
        }
    }
    m_FilterOptionChoice->SetSelection( 0 );

    m_FilterEditSizer->Layout();
}

// -------------------------------------------------------------------------------- //
void guDynPlayListEditor::OnFiltersSelected( wxCommandEvent &event )
{
    m_CurFilter = event.GetInt();
    if( m_CurFilter != wxNOT_FOUND )
    {
        guFilterItem * FilterItem = &( * m_Filters )[ m_CurFilter ];
        UpdateEditor( FilterItem->m_Type );
        m_FilterFieldChoice->SetSelection( FilterItem->m_Type );
        //guLogMessage( wxT( "Type : %u   Option: %u" ), FilterItem->m_Type, FilterItem->m_Option );
        m_FilterOptionChoice->SetSelection( FilterItem->m_Option );
        if( FilterItem->m_Type < guDYNAMIC_FILTER_TYPE_YEAR )
        {
            m_FilterText->SetValue( FilterItem->m_Text );
        }
        else if( FilterItem->m_Type == guDYNAMIC_FILTER_TYPE_RATING )
        {
            m_FilterRating->SetRating( FilterItem->m_Number );
        }
        else if( FilterItem->m_Type == guDYNAMIC_FILTER_TYPE_LENGTH )
        {
            unsigned long value = FilterItem->m_Number;
            m_LengthHours->SetValue( int( value / 3600 ) );
            value = value % 3600;
            m_LengthMinutes->SetValue( int( value / 60 ) );
            value = value % 60;
            m_LengthSeconds->SetValue( value );
        }
        else
        {
            m_FilterText->SetValue( wxString::Format( wxT( "%u" ), FilterItem->m_Number ) );
        }

        m_FilterDateOption2Choice->SetSelection( FilterItem->m_Option2 );
        //m_Filter
        m_FilterDel->Enable( true );
        m_FilterAccept->Enable( false );
        m_HasChanged = false;

    }
    else
    {
        m_FilterDel->Enable( false );
        m_FilterAccept->Enable( false );
    }
}

// -------------------------------------------------------------------------------- //
void guDynPlayListEditor::OnFilterFieldSelected( wxCommandEvent &event )
{
    int FilterType = m_FilterFieldChoice->GetSelection();

    m_FilterText->SetValue( wxEmptyString );
    m_LengthHours->SetValue( 0 );
    m_LengthMinutes->SetValue( 0 );
    m_LengthSeconds->SetValue( 0 );
    m_FilterRating->SetRating( 0 );

    UpdateEditor( FilterType );

    if( m_CurFilter != wxNOT_FOUND )
    {
        m_HasChanged = true;
        m_FilterAccept->Enable( true );
    }

}

// -------------------------------------------------------------------------------- //
void guDynPlayListEditor::OnFilterOptionSelected( wxCommandEvent &event )
{
    if( m_CurFilter != wxNOT_FOUND )
    {
        m_HasChanged = true;
        m_FilterAccept->Enable( true );
    }
}

// -------------------------------------------------------------------------------- //
void guDynPlayListEditor::OnFilterDateOption2Selected( wxCommandEvent &event )
{
    if( m_CurFilter != wxNOT_FOUND )
    {
        m_HasChanged = true;
        m_FilterAccept->Enable( true );
    }
}

// -------------------------------------------------------------------------------- //
void guDynPlayListEditor::OnFilterTextChanged( wxCommandEvent& event )
{
    int FilterType = m_FilterFieldChoice->GetSelection();
    if( FilterType > guDYNAMIC_FILTER_TYPE_PATH )
    {
        unsigned long value = 0;
        if( !m_FilterText->GetValue().IsEmpty() && !m_FilterText->GetValue().ToULong( &value ) )
            m_FilterText->SetValue( wxEmptyString );
    }


    m_FilterAdd->Enable( !m_FilterText->GetValue().IsEmpty() );
    if( m_CurFilter != wxNOT_FOUND )
    {
        m_HasChanged = true;
        m_FilterAccept->Enable( true );
    }
}

// -------------------------------------------------------------------------------- //
void guDynPlayListEditor::OnRatingChanged( guRatingEvent &event )
{
    m_FilterAdd->Enable( m_FilterRating->GetRating() >= 0 );
    if( m_CurFilter != wxNOT_FOUND )
    {
        m_HasChanged = true;
        m_FilterAccept->Enable( true );
    }
}

// -------------------------------------------------------------------------------- //
void guDynPlayListEditor::OnFilterAddClicked( wxCommandEvent& event )
{
    guFilterItem FilterItem = GetFilterItem();
    m_Filters->Add( FilterItem );
    m_FiltersListBox->Append( FilterItem.GetLabel() );
    m_FiltersListBox->Refresh();

    m_BtnOk->Enable( m_Filters->Count() );
}

// -------------------------------------------------------------------------------- //
void guDynPlayListEditor::OnFilterDelClicked( wxCommandEvent& event )
{
    if( m_CurFilter != wxNOT_FOUND )
    {
        m_Filters->RemoveAt( m_CurFilter );
        m_FiltersListBox->Delete( m_CurFilter );
        m_CurFilter = wxNOT_FOUND;

        m_BtnOk->Enable( m_Filters->Count() );
    }
}

// -------------------------------------------------------------------------------- //
void guDynPlayListEditor::OnFilterUpdateClicked( wxCommandEvent& event )
{
    if( m_CurFilter != wxNOT_FOUND )
    {
        guFilterItem FilterItem = GetFilterItem();
        ( * m_Filters )[ m_CurFilter ] = FilterItem;
        m_FiltersListBox->SetString( m_CurFilter, FilterItem.GetLabel() );
        m_FilterAccept->Enable( false );
    }
}

// -------------------------------------------------------------------------------- //
void guDynPlayListEditor::OnLImitChecked( wxCommandEvent &event )
{
	m_LimitChoice->Enable( m_LimitCheckBox->IsChecked() );
	m_LimitSpinCtrl->Enable( m_LimitCheckBox->IsChecked() );
}

// -------------------------------------------------------------------------------- //
void guDynPlayListEditor::OnSortChecked( wxCommandEvent &event )
{
	m_DescCheckBox->Enable( m_SortCheckBox->IsChecked() );
	m_SortChoice->Enable( m_SortCheckBox->IsChecked() );
}

// -------------------------------------------------------------------------------- //