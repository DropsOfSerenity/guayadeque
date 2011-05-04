// -------------------------------------------------------------------------------- //
//	Copyright (C) 2008-2011 J.Rios
//	anonbeat@gmail.com
//
//    This Program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 3, or (at your option)
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
#ifndef TREEPANEL_H
#define TREEPANEL_H

#include "AuiManagedPanel.h"
#include "DbLibrary.h"
#include "PlayerPanel.h"
#include "LibPanel.h"
#include "SoListBox.h"
#include "TreeViewFilter.h"
#include "TVSoListBox.h"

#include <wx/aui/aui.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/statline.h>
#include <wx/listctrl.h>
#include <wx/splitter.h>
#include <wx/frame.h>
#include <wx/treectrl.h>
#include <wx/imaglist.h>
#include <wx/srchctrl.h>

#define     guPANEL_TREEVIEW_TEXTSEARCH        ( 1 << 0 )

#define     guPANEL_TREEVIEW_VISIBLE_DEFAULT   ( 0 )


class guTreeViewPanel;

// -------------------------------------------------------------------------------- //
// guTreeViewTreeCtrl
// -------------------------------------------------------------------------------- //
class guTreeViewTreeCtrl : public wxTreeCtrl
{
  protected :
    guDbLibrary *       m_Db;
    guTreeViewPanel *   m_TreeViewPanel;

    wxArrayString       m_TextFilters;

    wxImageList *       m_ImageList;
    wxTreeItemId        m_RootId;
    wxTreeItemId        m_FiltersId;
    wxTreeItemId        m_LibraryId;

    wxArrayString       m_FilterEntries;
    wxArrayInt          m_FilterLayout;
    int                 m_CurrentFilter;

    void                OnContextMenu( wxTreeEvent &event );

    void                OnBeginDrag( wxTreeEvent &event );

    void                OnConfigUpdated( wxCommandEvent &event );
    void                CreateAcceleratorTable( void );

    void                LoadFilterLayout( void );

    void                OnSearchLinkClicked( wxCommandEvent &event );
    void                OnCommandClicked( wxCommandEvent &event );

  public :
    guTreeViewTreeCtrl( wxWindow * parent, guDbLibrary * db, guTreeViewPanel * playlistpanel );
    ~guTreeViewTreeCtrl();

    void                ReloadItems( void );
    void                ReloadFilters( void );

    void                SetCurrentFilter( const int curfilter ) { m_CurrentFilter = curfilter; LoadFilterLayout(); ReloadFilters(); }

    wxString            GetFilterEntry( const int index ) { return m_FilterEntries[ index ]; }
    void                AppendFilterEntry( const wxString &filterentry ) { m_FilterEntries.Add( filterentry ); ReloadFilters(); }
    void                DeleteFilterEntry( const int index ) { if( m_FilterEntries.Count() == 1 ) return; m_FilterEntries.RemoveAt( index ); ReloadFilters(); }
    void                SetFilterEntry( const int index, const wxString &filterentry ) { m_FilterEntries[ index ] = filterentry; ReloadFilters(); }

    void                GetItemFilterEntry( const wxTreeItemId &treeitemid, guTreeViewFilterEntry &filterentry );

    bool                IsFilterItem( const wxTreeItemId &item );
    bool                IsFilterEntry( const wxTreeItemId &item ) { return GetItemParent( item ) == m_FiltersId; }

    void                SetTextFilters( const wxArrayString &textfilters ) { m_TextFilters = textfilters; }
    void                ClearTextFilters( void ) { m_TextFilters.Clear(); }

    DECLARE_EVENT_TABLE()

    friend class guTreeViewPanel;
};

// -------------------------------------------------------------------------------- //
class guTreeViewPanel : public guAuiManagedPanel
{
  protected :
    guDbLibrary *           m_Db;
    guLibPanel *            m_LibPanel;
    guPlayerPanel *         m_PlayerPanel;

    wxSplitterWindow *      m_MainSplitter;
    guTreeViewTreeCtrl *    m_TreeViewCtrl;
    guTVSoListBox *         m_TVTracksListBox;

    wxSearchCtrl *          m_InputTextCtrl;

    wxTimer                 m_TextChangedTimer;
    wxTimer                 m_TreeItemSelectedTimer;

    bool                    m_InstantSearchEnabled;
    bool                    m_EnterSelectSearchEnabled;

    void                    OnTreeViewSelected( wxTreeEvent &event );
    void                    OnTreeViewActivated( wxTreeEvent &event );
    void                    OnTreeViewPlay( wxCommandEvent &event );
    void                    OnTreeViewEnqueue( wxCommandEvent &event );

    void                    OnTreeViewNewFilter( wxCommandEvent &event );
    void                    OnTreeViewEditFilter( wxCommandEvent &event );
    void                    OnTreeViewDeleteFilter( wxCommandEvent &event );
    void                    OnTreeViewEditLabels( wxCommandEvent &event );
    void                    OnTreeViewEditTracks( wxCommandEvent &event );
    void                    OnTreeViewSaveToPlayList( wxCommandEvent &event );
    void                    OnTreeViewCopyTo( wxCommandEvent &event );


    virtual void            OnTVTracksActivated( wxListEvent &event );
    void                    OnTVTracksPlayClicked( wxCommandEvent &event );
    void                    OnTVTracksQueueClicked( wxCommandEvent &event );
    void                    OnTVTracksEditLabelsClicked( wxCommandEvent &event );
    void                    OnTVTracksEditTracksClicked( wxCommandEvent &event );
    void                    OnTVTracksCopyToClicked( wxCommandEvent &event );
    void                    OnTVTracksSavePlayListClicked( wxCommandEvent &event );
    void                    OnTVTracksSetRating( wxCommandEvent &event );
    void                    OnTVTracksSetField( wxCommandEvent &event );
    void                    OnTVTracksEditField( wxCommandEvent &event );

    void                    OnTVTracksSelectGenre( wxCommandEvent &event );
    void                    OnTVTracksSelectAlbumArtist( wxCommandEvent &event );
    void                    OnTVTracksSelectArtist( wxCommandEvent &event );
    void                    OnTVTracksSelectAlbum( wxCommandEvent &event );

    void                    OnSearchActivated( wxCommandEvent &event );
    void                    OnSearchSelected( wxCommandEvent &event );
    void                    OnSearchCancelled( wxCommandEvent &event );

    void                    OnTextChangedTimer( wxTimerEvent &event );
    void                    OnTreeItemSelectedTimer( wxTimerEvent &event );

    void                    OnTVTracksDeleteLibrary( wxCommandEvent &event );
    void                    OnTVTracksDeleteDrive( wxCommandEvent &event );

    virtual void            NormalizeTracks( guTrackArray * tracks, const bool isdrag = false ) {};
    virtual void            SendPlayListUpdatedEvent( void );

    void                    OnGoToSearch( wxCommandEvent &event );
    bool                    DoTextSearch( void );

    void                    OnConfigUpdated( wxCommandEvent &event );

    guLibPanel *            GetLibPanel( void ) { return m_LibPanel; }

    void                    OnTrackListColClicked( wxListEvent &event );


  public :
    guTreeViewPanel( wxWindow * parent, guDbLibrary * db, guPlayerPanel * playerpanel, guLibPanel * libpanel );
    ~guTreeViewPanel();

    virtual void            InitPanelData( void );

    void                    GetTreeViewCounters( wxLongLong * count, wxLongLong * len, wxLongLong * size ) { m_TVTracksListBox->GetCounters( count, len, size ); }

    void inline             UpdatedTracks( const guTrackArray * tracks ) { m_TVTracksListBox->UpdatedTracks( tracks ); };
    void inline             UpdatedTrack( const guTrack * track ) { m_TVTracksListBox->UpdatedTrack( track ); };

    virtual int             GetListViewColumnCount( void ) { return guSONGS_COLUMN_COUNT; }
    virtual bool            GetListViewColumnData( const int id, int * index, int * width, bool * enabled ) { return m_TVTracksListBox->GetColumnData( id, index, width, enabled ); }
    virtual bool            SetListViewColumnData( const int id, const int index, const int width, const bool enabled, const bool refresh = false ) { return m_TVTracksListBox->SetColumnData( id, index, width, enabled, refresh ); }

    void                    GetAllTracks( guTrackArray * tracks ) { m_TVTracksListBox->GetAllSongs( tracks ); }

    friend class guTreeViewTreeCtrl;
};

#endif
// -------------------------------------------------------------------------------- //