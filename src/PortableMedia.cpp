// -------------------------------------------------------------------------------- //
//	Copyright (C) 2008-2010 J.Rios
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
#include "PortableMedia.h"
#include "MD5.h"
#include "Transcode.h"

#include <wx/tokenzr.h>

// -------------------------------------------------------------------------------- //
wxString guPortableMediaAudioFormatString[] = {
    wxT( "Keep format" ),
    wxT( "mp3" ),
    wxT( "ogg" ),
    wxT( "flac" ),
    wxT( "m4a" ),
    wxT( "wma" )
};

// -------------------------------------------------------------------------------- //
wxString guPortablePlaylistFormatString[] = {
    wxT( "m3u" ),
    wxT( "pls" ),
    wxT( "xspf" ),
    wxT( "asx" )
};

// -------------------------------------------------------------------------------- //
wxString guPortableCoverFormatString[] = {
    wxT( "jpeg" ),
    wxT( "png" ),
    wxT( "bmp" ),
    wxT( "gif" )
};


// -------------------------------------------------------------------------------- //
int guGetTranscodeFileFormat( const wxString &filetype )
{
    if( filetype == wxT( "mp3" ) )
    {
        return guPORTABLEMEDIA_AUDIO_FORMAT_MP3;
    }
    else if( filetype == wxT( "ogg" ) || filetype == wxT( "oga" ) )
    {
        return guPORTABLEMEDIA_AUDIO_FORMAT_OGG;
    }
    else if( filetype == wxT( "flac" ) )
    {
        return guPORTABLEMEDIA_AUDIO_FORMAT_FLAC;
    }
    else if( filetype == wxT( "m4a" ) || filetype == wxT( "m4b" ) ||
             filetype == wxT( "aac" ) || filetype == wxT( "mp4" ) ||
             filetype == wxT( "m4p" ) )
    {
        return guPORTABLEMEDIA_AUDIO_FORMAT_AAC;
    }
    else if( filetype == wxT( "wma" ) )
    {
        return guPORTABLEMEDIA_AUDIO_FORMAT_WMA;
    }
    return guTRANSCODE_FORMAT_KEEP;
}

// -------------------------------------------------------------------------------- //
wxString guGetTranscodeFormatString( const int format )
{
    return guPortableMediaAudioFormatString[ format ];
}

// -------------------------------------------------------------------------------- //
int MimeStrToAudioFormat( const wxString &mimestr )
{
    int Formats = 0;
    int Index;
    int Count;
    wxArrayString AudioFormats = wxStringTokenize( mimestr, wxT( "," ) );
    if( ( Count = AudioFormats.Count() ) )
    {
        for( Index = 0; Index < Count; Index++ )
        {
            AudioFormats[ Index ].Trim( true ).Trim( false );
            if( AudioFormats[ Index ] == wxT( "audio/mpeg" ) )
            {
                Formats |= guPORTABLEMEDIA_AUDIO_FORMAT_MP3;
            }
            else if( AudioFormats[ Index ] == wxT( "audio/ogg" ) )
            {
                Formats |= guPORTABLEMEDIA_AUDIO_FORMAT_OGG;
            }
            else if( AudioFormats[ Index ] == wxT( "audio/flac" ) )
            {
                Formats |= guPORTABLEMEDIA_AUDIO_FORMAT_FLAC;
            }
            else if( AudioFormats[ Index ] == wxT( "audio/mp4a-latm" ) )
            {
                Formats |= guPORTABLEMEDIA_AUDIO_FORMAT_AAC;
            }
            else if( AudioFormats[ Index ] == wxT( "audio/x-ms-wma" ) )
            {
                Formats |= guPORTABLEMEDIA_AUDIO_FORMAT_WMA;
            }
            else
            {
                guLogMessage( wxT( "Unknown audio mime type : '%s'" ), AudioFormats[ Index ].c_str() );
            }
        }
    }
    return Formats;
}

// -------------------------------------------------------------------------------- //
wxString AudioFormatsToMimeStr( const int formats )
{
    wxString MimeStr;
    if( formats & guPORTABLEMEDIA_AUDIO_FORMAT_MP3 )
    {
        MimeStr += wxT( "audio/mpeg" );
    }

    if( formats & guPORTABLEMEDIA_AUDIO_FORMAT_OGG )
    {
        if( !MimeStr.IsEmpty() )
        {
            MimeStr += wxT( ", " );
        }
        MimeStr += wxT( "audio/ogg" );
    }

    if( formats & guPORTABLEMEDIA_AUDIO_FORMAT_FLAC )
    {
        if( !MimeStr.IsEmpty() )
        {
            MimeStr += wxT( ", " );
        }
        MimeStr += wxT( "audio/flac" );
    }

    if( formats & guPORTABLEMEDIA_AUDIO_FORMAT_AAC )
    {
        if( !MimeStr.IsEmpty() )
        {
            MimeStr += wxT( ", " );
        }
        MimeStr += wxT( "audio/mp4a-latm" );
    }

    if( formats & guPORTABLEMEDIA_AUDIO_FORMAT_WMA )
    {
        if( !MimeStr.IsEmpty() )
        {
            MimeStr += wxT( ", " );
        }
        MimeStr += wxT( "audio/x-ms-wma" );
    }
    return MimeStr;
}

// -------------------------------------------------------------------------------- //
int MimeStrToPlaylistFormat( const wxString &mimestr )
{
    int Formats = 0;
    int Index;
    int Count;
    wxArrayString PlaylistFormats = wxStringTokenize( mimestr, wxT( "," ) );
    if( ( Count = PlaylistFormats.Count() ) )
    {
        for( Index = 0; Index < Count; Index++ )
        {
            PlaylistFormats[ Index ].Trim( true ).Trim( false );
            if( ( PlaylistFormats[ Index ] == wxT( "audio/mpegurl" ) ) ||
                     ( PlaylistFormats[ Index ] == wxT( "audio/x-mpegurl" ) ) )
            {
                Formats |= guPORTABLEMEDIA_PLAYLIST_FORMAT_M3U;
            }
            else if( PlaylistFormats[ Index ] == wxT( "audio/x-scpls" ) )
            {
                Formats |= guPORTABLEMEDIA_PLAYLIST_FORMAT_PLS;
            }
            else if( PlaylistFormats[ Index ] == wxT( "application/xspf+xml" ) )
            {
                Formats |= guPORTABLEMEDIA_PLAYLIST_FORMAT_XSPF;
            }
            else if( PlaylistFormats[ Index ] == wxT( "video/x-ms-asf" ) )
            {
                Formats |= guPORTABLEMEDIA_PLAYLIST_FORMAT_ASX;
            }
            else
            {
                guLogMessage( wxT( "Unknown playlist mime type : '%s'" ), PlaylistFormats[ Index ].c_str() );
            }
        }
    }
    return Formats;
}

// -------------------------------------------------------------------------------- //
wxString PlaylistFormatsToMimeStr( const int formats )
{
    wxString MimeStr;

    if( formats & guPORTABLEMEDIA_PLAYLIST_FORMAT_M3U )
    {
        MimeStr += wxT( "audio/mpegurl" );
    }

    if( formats & guPORTABLEMEDIA_PLAYLIST_FORMAT_PLS )
    {
        if( !MimeStr.IsEmpty() )
        {
            MimeStr += wxT( ", " );
        }
        MimeStr += wxT( "audio/x-scpls" );
    }

    if( formats & guPORTABLEMEDIA_PLAYLIST_FORMAT_XSPF )
    {
        if( !MimeStr.IsEmpty() )
        {
            MimeStr += wxT( ", " );
        }
        MimeStr += wxT( "application/xspf+xml" );
    }

    if( formats & guPORTABLEMEDIA_PLAYLIST_FORMAT_ASX )
    {
        if( !MimeStr.IsEmpty() )
        {
            MimeStr += wxT( ", " );
        }
        MimeStr += wxT( "video/x-ms-asf" );
    }

    return MimeStr;
}

// -------------------------------------------------------------------------------- //
int MimeStrToCoverFormat( const wxString &mimestr )
{
    int Formats = 0;
    int Index;
    int Count;
    wxArrayString CoverFormats = wxStringTokenize( mimestr, wxT( "," ) );
    if( ( Count = CoverFormats.Count() ) )
    {
        for( Index = 0; Index < Count; Index++ )
        {
            CoverFormats[ Index ].Trim( true ).Trim( false );
            if( CoverFormats[ Index ] == wxT( "image/jpeg" ) )
            {
                Formats |= guPORTABLEMEDIA_COVER_FORMAT_JPEG;
            }
            else if( ( CoverFormats[ Index ] == wxT( "image/png" ) ) )
            {
                Formats |= guPORTABLEMEDIA_COVER_FORMAT_PNG;
            }
            else if( CoverFormats[ Index ] == wxT( "image/bmp" ) )
            {
                Formats |= guPORTABLEMEDIA_COVER_FORMAT_BMP;
            }
            else if( CoverFormats[ Index ] == wxT( "image/gif" ) )
            {
                Formats |= guPORTABLEMEDIA_COVER_FORMAT_GIF;
            }
            else
            {
                guLogMessage( wxT( "Unknown playlist mime type : '%s'" ), CoverFormats[ Index ].c_str() );
            }
        }
    }
    return Formats;
}

// -------------------------------------------------------------------------------- //
wxString CoverFormatsToMimeStr( const int formats )
{
    wxString MimeStr;

    if( formats & guPORTABLEMEDIA_COVER_FORMAT_JPEG )
    {
        MimeStr += wxT( "image/jpeg" );
    }

    if( formats & guPORTABLEMEDIA_COVER_FORMAT_PNG )
    {
        if( !MimeStr.IsEmpty() )
        {
            MimeStr += wxT( ", " );
        }
        MimeStr += wxT( "image/png" );
    }

    if( formats & guPORTABLEMEDIA_COVER_FORMAT_BMP )
    {
        if( !MimeStr.IsEmpty() )
        {
            MimeStr += wxT( ", " );
        }
        MimeStr += wxT( "image/bmp" );
    }

    if( formats & guPORTABLEMEDIA_COVER_FORMAT_GIF )
    {
        if( !MimeStr.IsEmpty() )
        {
            MimeStr += wxT( ", " );
        }
        MimeStr += wxT( "image/gif" );
    }

    return MimeStr;
}

// -------------------------------------------------------------------------------- //
guPortableMediaDevice::guPortableMediaDevice( guGIO_Mount * mount )
{
    m_Mount = mount;

    guConfig * Config = new guConfig( m_Mount->GetMountPath() + wxT( "/.is_audio_player" ) );
    guMD5 MD5;
    m_Id = Config->ReadStr( wxT( "audio_player_id" ), MD5.MD5( wxString::Format( wxT( "%i" ), wxGetLocalTime() ) ).Right( 6 ) );
    m_Pattern = Config->ReadStr( wxT( "audio_file_pattern" ), wxT( "{a} - {b}/{n} - {a} - {t}" ) );
    m_AudioFormats = MimeStrToAudioFormat( Config->ReadStr( wxT( "output_formats" ), wxT( "mp3" ) ) );
    m_TranscodeFormat = Config->ReadNum( wxT( "transcode_format" ), guTRANSCODE_FORMAT_KEEP );
    m_TranscodeScope = Config->ReadNum( wxT( "transcode_scope" ), guPORTABLEMEDIA_TRANSCODE_SCOPE_NOT_SUPPORTED );
    m_TranscodeQuality = Config->ReadNum( wxT( "transcode_quality" ), guTRANSCODE_QUALITY_KEEP );
    m_AudioFolders = Config->ReadStr( wxT( "audio_folders" ), wxT( "/" ) );
    m_PlaylistFormats = MimeStrToPlaylistFormat( Config->ReadStr( wxT( "playlist_format" ), wxEmptyString ) );
    m_PlaylistFolder = Config->ReadStr( wxT( "playlist_path" ), wxEmptyString );
    m_CoverFormats = MimeStrToCoverFormat( Config->ReadStr( wxT( "cover_art_file_type" ), wxT( "jpeg" ) ) );
    m_CoverName = Config->ReadStr( wxT( "cover_art_file_name" ), wxT( "cover" ) );
    m_CoverSize = Config->ReadNum( wxT( "cover_art_size" ), 100 );

    wxGetDiskSpace( m_Mount->GetMountPath(), &m_DiskSize, &m_DiskFree );

    Config->WriteStr( wxT( "audio_player_id" ), m_Id );
    Config->Flush();

    delete Config;
}

// -------------------------------------------------------------------------------- //
guPortableMediaDevice::~guPortableMediaDevice()
{
}

// -------------------------------------------------------------------------------- //
void guPortableMediaDevice::WriteConfig( void )
{
    guConfig * Config = new guConfig( m_Mount->GetMountPath() + wxT( "/.is_audio_player" ) );
    Config->WriteStr( wxT( "audio_file_pattern" ), m_Pattern );
    Config->WriteStr( wxT( "output_formats" ), AudioFormatsToMimeStr( m_AudioFormats ) );
    Config->WriteNum( wxT( "transcode_format" ), m_TranscodeFormat );
    Config->WriteNum( wxT( "transcode_scope" ), m_TranscodeScope );
    Config->WriteNum( wxT( "transcode_quality" ), m_TranscodeQuality );
    Config->WriteStr( wxT( "audio_folders" ), m_AudioFolders );
    Config->WriteStr( wxT( "playlist_format" ), PlaylistFormatsToMimeStr( m_PlaylistFormats ) );
    Config->WriteStr( wxT( "playlist_path" ), m_PlaylistFolder );
    Config->WriteStr( wxT( "cover_art_file_type" ), CoverFormatsToMimeStr( m_CoverFormats ) );
    Config->WriteStr( wxT( "cover_art_file_name" ), m_CoverName );
    Config->WriteNum( wxT( "cover_art_size" ), m_CoverSize );
    Config->Flush();

    delete Config;

}

// -------------------------------------------------------------------------------- //
wxString guPortableMediaDevice::AudioFormatsStr( const int formats )
{
    wxString AudioFormats;
    if( formats & guPORTABLEMEDIA_AUDIO_FORMAT_MP3 )
    {
        AudioFormats += wxT( "mp3" );
    }

    if( formats & guPORTABLEMEDIA_AUDIO_FORMAT_OGG )
    {
        if( !AudioFormats.IsEmpty() )
            AudioFormats += wxT( ", " );
        AudioFormats += wxT( "ogg" );
    }

    if( formats & guPORTABLEMEDIA_AUDIO_FORMAT_FLAC )
    {
        if( !AudioFormats.IsEmpty() )
            AudioFormats += wxT( ", " );
        AudioFormats += wxT( "flac" );
    }

    if( formats & guPORTABLEMEDIA_AUDIO_FORMAT_AAC )
    {
        if( !AudioFormats.IsEmpty() )
            AudioFormats += wxT( ", " );
        AudioFormats += wxT( "aac" );
    }

    if( formats & guPORTABLEMEDIA_AUDIO_FORMAT_WMA )
    {
        if( !AudioFormats.IsEmpty() )
            AudioFormats += wxT( ", " );
        AudioFormats += wxT( "wma" );
    }
    return AudioFormats;
}


// -------------------------------------------------------------------------------- //
wxString guPortableMediaDevice::PlaylistFormatsStr( const int formats )
{
    wxString Formats;
    if( formats & guPORTABLEMEDIA_PLAYLIST_FORMAT_M3U )
    {
        Formats += wxT( "m3u" );
    }

    if( formats & guPORTABLEMEDIA_PLAYLIST_FORMAT_PLS )
    {
        if( !Formats.IsEmpty() )
            Formats += wxT( ", " );
        Formats += wxT( "pls" );
    }

    if( formats & guPORTABLEMEDIA_PLAYLIST_FORMAT_XSPF )
    {
        if( !Formats.IsEmpty() )
            Formats += wxT( ", " );
        Formats += wxT( "xspf" );
    }

    if( formats & guPORTABLEMEDIA_PLAYLIST_FORMAT_ASX )
    {
        if( !Formats.IsEmpty() )
            Formats += wxT( ", " );
        Formats += wxT( "asx" );
    }

    return Formats;
}

// -------------------------------------------------------------------------------- //
wxString guPortableMediaDevice::CoverFormatsStr( const int formats )
{
    wxString Formats;
    if( formats & guPORTABLEMEDIA_COVER_FORMAT_JPEG )
    {
        Formats += wxT( "jpeg" );
    }

    if( formats & guPORTABLEMEDIA_COVER_FORMAT_PNG )
    {
        if( !Formats.IsEmpty() )
            Formats += wxT( ", " );
        Formats += wxT( "png" );
    }

    if( formats & guPORTABLEMEDIA_COVER_FORMAT_BMP )
    {
        if( !Formats.IsEmpty() )
            Formats += wxT( ", " );
        Formats += wxT( "bmp" );
    }

    if( formats & guPORTABLEMEDIA_COVER_FORMAT_GIF )
    {
        if( !Formats.IsEmpty() )
            Formats += wxT( ", " );
        Formats += wxT( "gif" );
    }

    return Formats;
}

// -------------------------------------------------------------------------------- //
// guPortableMediaLibrary
// -------------------------------------------------------------------------------- //
guPortableMediaLibrary::guPortableMediaLibrary( const wxString &dbname ) :
    guDbLibrary( dbname )
{
}

// -------------------------------------------------------------------------------- //
guPortableMediaLibrary::~guPortableMediaLibrary()
{
}

// -------------------------------------------------------------------------------- //
void guPortableMediaLibrary::CreateNewSong( guTrack * track )
{
}


// -------------------------------------------------------------------------------- //
// guPortableMediaPanel
// -------------------------------------------------------------------------------- //
guPortableMediaPanel::guPortableMediaPanel( wxWindow * parent, guPortableMediaLibrary * db, guPlayerPanel * playerpanel, const wxString &prefix ) :
    guLibPanel( parent, db, playerpanel, prefix )
{
    //SetBaseCommand( ID_MENU_VIEW_PORTABLE_DEVICES );

    //m_ContextMenuFlags = ( guLIBRARY_CONTEXTMENU_DOWNLOAD_COVERS | guLIBRARY_CONTEXTMENU_LINKS );

    guConfig * Config = ( guConfig * ) guConfig::Get();
    Config->RegisterObject( this ); // Get notified when configuration changes

    Connect( ID_PORTABLEDEVICE_UPDATE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( guPortableMediaPanel::OnPortableLibraryUpdate ), NULL, this );
    Connect( ID_PORTABLEDEVICE_UNMOUNT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( guPortableMediaPanel::OnPortableUnmount ), NULL, this );
    Connect( ID_PORTABLEDEVICE_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( guPortableMediaPanel::OnPortableProperties ), NULL, this );

}

// -------------------------------------------------------------------------------- //
guPortableMediaPanel::~guPortableMediaPanel()
{
    Disconnect( ID_PORTABLEDEVICE_UPDATE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( guPortableMediaPanel::OnPortableLibraryUpdate ), NULL, this );
    Disconnect( ID_PORTABLEDEVICE_UNMOUNT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( guPortableMediaPanel::OnPortableUnmount ), NULL, this );
    Disconnect( ID_PORTABLEDEVICE_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( guPortableMediaPanel::OnPortableProperties ), NULL, this );
}

// -------------------------------------------------------------------------------- //
void guPortableMediaPanel::NormalizeTracks( guTrackArray * tracks, const bool isdrag )
{
    int Index;
    int Count;
    if( tracks && ( Count = tracks->Count() ) )
    {
        for( Index = 0; Index < Count; Index++ )
        {
            guTrack * Track = &( * tracks )[ Index ];
            Track->m_LibPanel = this;
        }
    }
}

// -------------------------------------------------------------------------------- //
void guPortableMediaPanel::CreateContextMenu( wxMenu * menu, const int windowid )
{
    wxMenu *     SubMenu = new wxMenu();

    wxMenuItem * MenuItem = new wxMenuItem( menu, ID_PORTABLEDEVICE_UPDATE, _( "Update" ), _( "Update the device library" ) );
    SubMenu->Append( MenuItem );

    MenuItem = new wxMenuItem( menu, ID_PORTABLEDEVICE_PROPERTIES, _( "Properties" ), _( "Set the device properties" ) );
    SubMenu->Append( MenuItem );

    MenuItem = new wxMenuItem( menu, ID_PORTABLEDEVICE_UNMOUNT, _( "Unmount" ), _( "Unmount the device" ) );
    SubMenu->Append( MenuItem );

    menu->AppendSeparator();
    menu->AppendSubMenu( SubMenu, _( "Portable Device" ), _( "Global Jamendo options" ) );
}

// -------------------------------------------------------------------------------- //
wxString guPortableMediaPanel::GetName( void )
{
    return m_PortableMediaDevice->DeviceName();
}

// -------------------------------------------------------------------------------- //
wxArrayString guPortableMediaPanel::GetPaths( void )
{
    wxArrayString Paths = wxStringTokenize( m_PortableMediaDevice->AudioFolders(), wxT( "," ) );
    int Index;
    int Count = Paths.Count();
    for( Index = 0; Index < Count; Index++ )
    {
        Paths[ Index ] = m_PortableMediaDevice->MountPath() + Paths[ Index ];
    }
    return Paths;
}

// -------------------------------------------------------------------------------- //
void guPortableMediaPanel::OnPortableLibraryUpdate( wxCommandEvent &event )
{
    event.SetId( ID_MENU_UPDATE_LIBRARY );
    event.SetClientData( ( void * ) this );
    wxPostEvent( wxTheApp->GetTopWindow(), event );
}

// -------------------------------------------------------------------------------- //
void guPortableMediaPanel::OnPortableProperties( wxCommandEvent &event )
{
    guPortableMediaProperties * PortableMediaProperties = new guPortableMediaProperties( this, m_PortableMediaDevice );
    if( PortableMediaProperties )
    {
        if( PortableMediaProperties->ShowModal() == wxID_OK )
        {
            PortableMediaProperties->WriteConfig();
        }
        PortableMediaProperties->Destroy();
    }
}

// -------------------------------------------------------------------------------- //
void guPortableMediaPanel::OnPortableUnmount( wxCommandEvent &event )
{
    if( m_PortableMediaDevice->CanUnmount() )
    {
        m_PortableMediaDevice->Unmount();
    }
}

// -------------------------------------------------------------------------------- //
int guPortableMediaPanel::LastUpdate( void )
{
    return 0;
}

// -------------------------------------------------------------------------------- //
void guPortableMediaPanel::SetLastUpdate( int lastupdate )
{
    // The portable media devices cant set lastupdate
}



// -------------------------------------------------------------------------------- //
// guPortableMediaProperties
// -------------------------------------------------------------------------------- //
guPortableMediaProperties::guPortableMediaProperties( wxWindow * parent, guPortableMediaDevice * mediadevice )
{
    wxStaticText *              NameLabel;
    wxStaticText *              MountPathLabel;
    wxStaticText *              UsedLabel;
    wxNotebook *                PMNotebook;
    wxPanel *                   PMAudioPanel;
    wxStaticText *              NamePatternLabel;

    wxStaticText *              AudioFolderLabel;
    wxStaticText *              AudioFormatLabel;

    wxPanel *                   PMPlaylistPanel;
    wxStaticText *              PlaylistFormatLabel;
    wxStaticText *              PlaylistFolderLabel;
    wxPanel *                   PMCoversPanel;
    wxStaticText *              CoverFormatLabel;
    wxStaticText *              CoverNameLabel;

    wxStaticText *              CoverSizeLabel;
    wxStdDialogButtonSizer *    BtnSizer;
    wxButton *                  BtnSizerOK;
    wxButton *                  BtnSizerCancel;


    m_PortableMediaDevice = mediadevice;

    guConfig * Config = ( guConfig * ) guConfig::Get();
    wxPoint WindowPos;
    WindowPos.x = Config->ReadNum( wxT( "PMPropertiesPosX" ), -1, wxT( "Positions" ) );
    WindowPos.y = Config->ReadNum( wxT( "PMPropertiesPosY" ), -1, wxT( "Positions" ) );
    wxSize WindowSize;
    WindowSize.x = Config->ReadNum( wxT( "PMPropertiesWidth" ), 570, wxT( "Positions" ) );
    WindowSize.y = Config->ReadNum( wxT( "PMPropertiesHeight" ), 420, wxT( "Positions" ) );

    Create( parent, wxID_ANY, _( "Portable Media Properties" ), WindowPos, WindowSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER );

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* MainSizer;
	MainSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* PMBoxSizer;
	PMBoxSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _( " Properties " ) ), wxVERTICAL );

	wxFlexGridSizer* PMFlexSizer;
	PMFlexSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
	PMFlexSizer->AddGrowableCol( 1 );
	PMFlexSizer->SetFlexibleDirection( wxBOTH );
	PMFlexSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	NameLabel = new wxStaticText( this, wxID_ANY, _("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	NameLabel->Wrap( -1 );
	NameLabel->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );

	PMFlexSizer->Add( NameLabel, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_NameText = new wxStaticText( this, wxID_ANY, mediadevice->DeviceName(), wxDefaultPosition, wxDefaultSize, 0 );
	m_NameText->Wrap( -1 );
	PMFlexSizer->Add( m_NameText, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );

	MountPathLabel = new wxStaticText( this, wxID_ANY, _("Path:"), wxDefaultPosition, wxDefaultSize, 0 );
	MountPathLabel->Wrap( -1 );
	MountPathLabel->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );

	PMFlexSizer->Add( MountPathLabel, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_MountPathText = new wxStaticText( this, wxID_ANY, mediadevice->MountPath(), wxDefaultPosition, wxDefaultSize, 0 );
	m_MountPathText->Wrap( -1 );
	PMFlexSizer->Add( m_MountPathText, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );

	UsedLabel = new wxStaticText( this, wxID_ANY, _("Used:"), wxDefaultPosition, wxDefaultSize, 0 );
	UsedLabel->Wrap( -1 );
	PMFlexSizer->Add( UsedLabel, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_UsedGauge = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	m_UsedGauge->SetValue( ( m_PortableMediaDevice->DiskSize() - m_PortableMediaDevice->DiskFree() ) * double( 100 ) / m_PortableMediaDevice->DiskSize() );
	PMFlexSizer->Add( m_UsedGauge, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	guLogMessage( wxT( "Disk %.0f free of %.0f" ), m_PortableMediaDevice->DiskFree(), m_PortableMediaDevice->DiskSize() );

	PMBoxSizer->Add( PMFlexSizer, 0, wxEXPAND, 5 );

	PMNotebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	PMAudioPanel = new wxPanel( PMNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	PMFlexSizer = new wxFlexGridSizer( 5, 3, 0, 0 );
	PMFlexSizer->AddGrowableCol( 1 );
	PMFlexSizer->SetFlexibleDirection( wxBOTH );
	PMFlexSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	NamePatternLabel = new wxStaticText( PMAudioPanel, wxID_ANY, _("Name Pattern:"), wxDefaultPosition, wxDefaultSize, 0 );
	NamePatternLabel->Wrap( -1 );
	PMFlexSizer->Add( NamePatternLabel, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_NamePatternText = new wxTextCtrl( PMAudioPanel, wxID_ANY, mediadevice->Pattern(), wxDefaultPosition, wxDefaultSize, 0 );
	PMFlexSizer->Add( m_NamePatternText, 0, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5 );


	PMFlexSizer->Add( 0, 0, 0, wxEXPAND, 5 );

	AudioFolderLabel = new wxStaticText( PMAudioPanel, wxID_ANY, _("Audio Folders:"), wxDefaultPosition, wxDefaultSize, 0 );
	AudioFolderLabel->Wrap( -1 );
	PMFlexSizer->Add( AudioFolderLabel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_AudioFolderText = new wxTextCtrl( PMAudioPanel, wxID_ANY, mediadevice->AudioFolders(), wxDefaultPosition, wxDefaultSize, 0 );
	PMFlexSizer->Add( m_AudioFolderText, 0, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5 );

	m_AudioFolderBtn = new wxButton( PMAudioPanel, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize( 28,-1 ), 0 );
	PMFlexSizer->Add( m_AudioFolderBtn, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );

	AudioFormatLabel = new wxStaticText( PMAudioPanel, wxID_ANY, _("Audio Formats:"), wxDefaultPosition, wxDefaultSize, 0 );
	AudioFormatLabel->Wrap( -1 );
	PMFlexSizer->Add( AudioFormatLabel, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_AudioFormats = mediadevice->AudioFormats();
	m_AudioFormatText = new wxTextCtrl( PMAudioPanel, wxID_ANY, mediadevice->AudioFormatsStr(), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	PMFlexSizer->Add( m_AudioFormatText, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxTOP|wxBOTTOM, 5 );

	m_AudioFormatBtn = new wxButton( PMAudioPanel, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize( 28,-1 ), 0 );
	PMFlexSizer->Add( m_AudioFormatBtn, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );

	wxStaticText * TransFormatLabel = new wxStaticText( PMAudioPanel, wxID_ANY, _("Transcode to:"), wxDefaultPosition, wxDefaultSize, 0 );
	TransFormatLabel->Wrap( -1 );
	PMFlexSizer->Add( TransFormatLabel, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxBoxSizer* TranscodeSizer;
	TranscodeSizer = new wxBoxSizer( wxHORIZONTAL );

	int m_TransFormatChoiceNChoices = sizeof( guPortableMediaAudioFormatString ) / sizeof( wxString );
	m_TransFormatChoice = new wxChoice( PMAudioPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_TransFormatChoiceNChoices, guPortableMediaAudioFormatString, 0 );
	m_TransFormatChoice->SetSelection( mediadevice->TranscodeFormat() );
	TranscodeSizer->Add( m_TransFormatChoice, 1, wxEXPAND|wxTOP|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_TransScopeChoiceChoices[] = { _("Unsupported formats only"), _("always") };
	int m_TransScopeChoiceNChoices = sizeof( m_TransScopeChoiceChoices ) / sizeof( wxString );
	m_TransScopeChoice = new wxChoice( PMAudioPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_TransScopeChoiceNChoices, m_TransScopeChoiceChoices, 0 );
	m_TransScopeChoice->SetSelection( mediadevice->TranscodeScope() );
	m_TransScopeChoice->Enable( mediadevice->TranscodeFormat() != guTRANSCODE_FORMAT_KEEP );
	TranscodeSizer->Add( m_TransScopeChoice, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

	PMFlexSizer->Add( TranscodeSizer, 1, wxEXPAND, 5 );


	PMFlexSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	wxStaticText * TransQualityLabel = new wxStaticText( PMAudioPanel, wxID_ANY, _("Quality:"), wxDefaultPosition, wxDefaultSize, 0 );
	TransQualityLabel->Wrap( -1 );
	PMFlexSizer->Add( TransQualityLabel, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxArrayString QualityStrings;
    QualityStrings.Add( _( "Keep quality" ) );
	QualityStrings.Add( _( "Very High" ) );
	QualityStrings.Add( _( "High" ) );
	QualityStrings.Add( _( "Very Good" ) );
	QualityStrings.Add( _( "Good" ) );
	QualityStrings.Add( _( "Normal" ) );
	QualityStrings.Add( _( "Low" ) );
	QualityStrings.Add( _( "Very Low" ) );

	m_TransQualityChoice = new wxChoice( PMAudioPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, QualityStrings, 0 );
	m_TransQualityChoice->SetSelection( mediadevice->TranscodeQuality() );
    m_TransQualityChoice->Enable( ( m_PortableMediaDevice->TranscodeFormat() != guTRANSCODE_FORMAT_KEEP ) &&
                                  ( m_PortableMediaDevice->TranscodeQuality() != guTRANSCODE_QUALITY_KEEP ) );
	PMFlexSizer->Add( m_TransQualityChoice, 0, wxEXPAND|wxTOP|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 5 );


	PMFlexSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	PMAudioPanel->SetSizer( PMFlexSizer );
	PMAudioPanel->Layout();
	PMFlexSizer->Fit( PMAudioPanel );
	PMNotebook->AddPage( PMAudioPanel, _("Audio"), true );
	PMPlaylistPanel = new wxPanel( PMNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	PMFlexSizer = new wxFlexGridSizer( 6, 3, 0, 0 );
	PMFlexSizer->AddGrowableCol( 1 );
	PMFlexSizer->SetFlexibleDirection( wxBOTH );
	PMFlexSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	PlaylistFolderLabel = new wxStaticText( PMPlaylistPanel, wxID_ANY, _("PlayList Folders:"), wxDefaultPosition, wxDefaultSize, 0 );
	PlaylistFolderLabel->Wrap( -1 );
	PMFlexSizer->Add( PlaylistFolderLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxALIGN_RIGHT, 5 );

	m_PlaylistFolderText = new wxTextCtrl( PMPlaylistPanel, wxID_ANY, mediadevice->PlaylistFolder(), wxDefaultPosition, wxDefaultSize, 0 );
	PMFlexSizer->Add( m_PlaylistFolderText, 0, wxEXPAND|wxTOP|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 5 );

	m_PlaylistFolderBtn = new wxButton( PMPlaylistPanel, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize( 28,-1 ), 0 );
	PMFlexSizer->Add( m_PlaylistFolderBtn, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );

	PlaylistFormatLabel = new wxStaticText( PMPlaylistPanel, wxID_ANY, _("PlayList Format:"), wxDefaultPosition, wxDefaultSize, 0 );
	PlaylistFormatLabel->Wrap( -1 );
	PMFlexSizer->Add( PlaylistFormatLabel, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    m_PlaylistFormats = mediadevice->PlaylistFormats();
	m_PlaylistFormatText = new wxTextCtrl( PMPlaylistPanel, wxID_ANY, mediadevice->PlaylistFormatsStr(), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	PMFlexSizer->Add( m_PlaylistFormatText, 0, wxEXPAND|wxTOP|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 5 );

	m_PlaylistFormatBtn = new wxButton( PMPlaylistPanel, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize( 28,-1 ), 0 );
	PMFlexSizer->Add( m_PlaylistFormatBtn, 0, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	PMPlaylistPanel->SetSizer( PMFlexSizer );
	PMPlaylistPanel->Layout();
	PMFlexSizer->Fit( PMPlaylistPanel );
	PMNotebook->AddPage( PMPlaylistPanel, _("Playlists"), false );
	PMCoversPanel = new wxPanel( PMNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	PMFlexSizer = new wxFlexGridSizer( 6, 3, 0, 0 );
	PMFlexSizer->AddGrowableCol( 1 );
	PMFlexSizer->SetFlexibleDirection( wxBOTH );
	PMFlexSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	CoverFormatLabel = new wxStaticText( PMCoversPanel, wxID_ANY, wxT("Cover Format:"), wxDefaultPosition, wxDefaultSize, 0 );
	CoverFormatLabel->Wrap( -1 );
	PMFlexSizer->Add( CoverFormatLabel, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    m_CoverFormats = mediadevice->CoverFormats();
	m_CoverFormatText = new wxTextCtrl( PMCoversPanel, wxID_ANY, mediadevice->CoverFormatsStr(), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	PMFlexSizer->Add( m_CoverFormatText, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxTOP|wxBOTTOM, 5 );

	m_CoverFormatBtn = new wxButton( PMCoversPanel, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize( 28,-1 ), 0 );
	PMFlexSizer->Add( m_CoverFormatBtn, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );

	CoverNameLabel = new wxStaticText( PMCoversPanel, wxID_ANY, wxT("Cover Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	CoverNameLabel->Wrap( -1 );
	PMFlexSizer->Add( CoverNameLabel, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_CoverNameText = new wxTextCtrl( PMCoversPanel, wxID_ANY, mediadevice->CoverName(), wxDefaultPosition, wxDefaultSize, 0 );
	PMFlexSizer->Add( m_CoverNameText, 0, wxEXPAND|wxTOP|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 5 );


	PMFlexSizer->Add( 0, 0, 0, wxEXPAND, 5 );

	CoverSizeLabel = new wxStaticText( PMCoversPanel, wxID_ANY, wxT("Covers Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	CoverSizeLabel->Wrap( -1 );
	PMFlexSizer->Add( CoverSizeLabel, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_CoverSizeText = new wxTextCtrl( PMCoversPanel, wxID_ANY, wxString::Format( wxT( "%u" ), mediadevice->CoverSize() ), wxDefaultPosition, wxDefaultSize, 0 );
	PMFlexSizer->Add( m_CoverSizeText, 0, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	PMCoversPanel->SetSizer( PMFlexSizer );
	PMCoversPanel->Layout();
	PMFlexSizer->Fit( PMCoversPanel );
	PMNotebook->AddPage( PMCoversPanel, wxT("Covers"), false );

	PMBoxSizer->Add( PMNotebook, 1, wxEXPAND | wxALL, 5 );

	MainSizer->Add( PMBoxSizer, 1, wxEXPAND|wxALL, 5 );

	BtnSizer = new wxStdDialogButtonSizer();
	BtnSizerOK = new wxButton( this, wxID_OK );
	BtnSizer->AddButton( BtnSizerOK );
	BtnSizerCancel = new wxButton( this, wxID_CANCEL );
	BtnSizer->AddButton( BtnSizerCancel );
	BtnSizer->Realize();
	MainSizer->Add( BtnSizer, 0, wxEXPAND|wxALL, 5 );

	this->SetSizer( MainSizer );
	this->Layout();

	m_AudioFolderBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( guPortableMediaProperties::OnAudioFolderBtnClick ), NULL, this );
	m_AudioFormatBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( guPortableMediaProperties::OnAudioFormatBtnClick ), NULL, this );

	m_TransFormatChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( guPortableMediaProperties::OnTransFormatChanged ), NULL, this );

	m_PlaylistFolderBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( guPortableMediaProperties::OnPlaylistFolderBtnClick ), NULL, this );
	m_PlaylistFormatBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( guPortableMediaProperties::OnPlaylistFormatBtnClick ), NULL, this );


	m_CoverFormatBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( guPortableMediaProperties::OnCoverFormatBtnClick ), NULL, this );

}

// -------------------------------------------------------------------------------- //
guPortableMediaProperties::~guPortableMediaProperties()
{
    guConfig * Config = ( guConfig * ) guConfig::Get();
    wxPoint WindowPos = GetPosition();
    Config->WriteNum( wxT( "PMPropertiesPosX" ), WindowPos.x, wxT( "Positions" ) );
    Config->WriteNum( wxT( "PMPropertiesPosY" ), WindowPos.y, wxT( "Positions" ) );
    wxSize WindowSize = GetSize();
    Config->WriteNum( wxT( "PMPropertiesWidth" ), WindowSize.x, wxT( "Positions" ) );
    Config->WriteNum( wxT( "PMPropertiesHeight" ), WindowSize.y, wxT( "Positions" ) );
    Config->Flush();

}

// -------------------------------------------------------------------------------- //
void guPortableMediaProperties::WriteConfig( void )
{
    m_PortableMediaDevice->SetPattern( m_NamePatternText->GetValue() );
    m_PortableMediaDevice->SetAudioFormats( m_AudioFormats );
    m_PortableMediaDevice->SetTranscodeFormat( m_TransFormatChoice->GetSelection() );
    m_PortableMediaDevice->SetTranscodeScope( m_TransScopeChoice->GetSelection() );
    m_PortableMediaDevice->SetTranscodeQuality( m_TransQualityChoice->GetSelection() );
    m_PortableMediaDevice->SetAudioFolders( m_AudioFolderText->GetValue() );
    m_PortableMediaDevice->SetPlaylistFormats( m_PlaylistFormats );
    m_PortableMediaDevice->SetPlaylistFolder( m_PlaylistFolderText->GetValue() );
    m_PortableMediaDevice->SetCoverFormats( m_CoverFormats );
    m_PortableMediaDevice->SetCoverName( m_CoverNameText->GetValue() );
    long CoverSize;
    m_CoverSizeText->GetValue().ToLong( &CoverSize );
    m_PortableMediaDevice->SetCoverSize( CoverSize );

    m_PortableMediaDevice->WriteConfig();
}

// -------------------------------------------------------------------------------- //
void guPortableMediaProperties::OnAudioFolderBtnClick( wxCommandEvent &event )
{
    wxString MountPath = m_PortableMediaDevice->MountPath() + wxT( "/" );

    wxDirDialog * DirDialog = new wxDirDialog( this,
                        _( "Select audio folder" ), MountPath, wxDD_DIR_MUST_EXIST );
    if( DirDialog )
    {
        if( DirDialog->ShowModal() == wxID_OK )
        {
            wxString NewFolder = DirDialog->GetPath() + wxT( "/" );
            if( NewFolder.StartsWith( MountPath ) )
            {
                NewFolder = NewFolder.Mid( MountPath.Length() );

                wxString AudioFolders = m_AudioFolderText->GetValue();

                if( AudioFolders.Find( NewFolder ) == wxNOT_FOUND )
                {
                    if( !AudioFolders.IsEmpty() )
                        AudioFolders.Append( wxT( ", " ) );

                    AudioFolders.Append( NewFolder );

                    m_AudioFolderText->SetValue( AudioFolders );
                }
            }
        }
        DirDialog->Destroy();
    }
}

// -------------------------------------------------------------------------------- //
void guPortableMediaProperties::OnAudioFormatBtnClick( wxCommandEvent &event )
{
    wxArrayInt Selection;
    wxArrayString Items;
    Items.Add( guPortableMediaAudioFormatString[ 1 ] );
    Items.Add( guPortableMediaAudioFormatString[ 2 ] );
    Items.Add( guPortableMediaAudioFormatString[ 3 ] );
    Items.Add( guPortableMediaAudioFormatString[ 4 ] );
    Items.Add( guPortableMediaAudioFormatString[ 5 ] );

    wxArrayInt ItemFlags;
    ItemFlags.Add( guPORTABLEMEDIA_AUDIO_FORMAT_MP3 );
    ItemFlags.Add( guPORTABLEMEDIA_AUDIO_FORMAT_OGG );
    ItemFlags.Add( guPORTABLEMEDIA_AUDIO_FORMAT_FLAC );
    ItemFlags.Add( guPORTABLEMEDIA_AUDIO_FORMAT_AAC );
    ItemFlags.Add( guPORTABLEMEDIA_AUDIO_FORMAT_WMA );

    int Index;
    int Count = ItemFlags.Count();
    for( Index = 0; Index < Count; Index++ )
    {
        if( m_AudioFormats & ItemFlags[ Index ] )
        {
            Selection.Add( Index );
        }
    }

    guListCheckOptionsDialog * ListCheckOptionsDialog = new guListCheckOptionsDialog( this, _( "Audio format" ), Items, Selection );
    if( ListCheckOptionsDialog )
    {
        if( ListCheckOptionsDialog->ShowModal() == wxID_OK )
        {
            ListCheckOptionsDialog->GetSelectedItems( Selection );
            m_AudioFormats = 0;
            Count = Selection.Count();
            for( Index = 0; Index < Count; Index++ )
            {
                m_AudioFormats |= ItemFlags[ Selection[ Index ] ];
            }
            m_AudioFormatText->SetValue( m_PortableMediaDevice->AudioFormatsStr( m_AudioFormats ) );
        }
        ListCheckOptionsDialog->Destroy();
    }
}

// -------------------------------------------------------------------------------- //
void guPortableMediaProperties::OnPlaylistFolderBtnClick( wxCommandEvent &event )
{
    wxString MountPath = m_PortableMediaDevice->MountPath() + wxT( "/" );

    wxDirDialog * DirDialog = new wxDirDialog( this,
                        _( "Select audio folder" ), MountPath, wxDD_DIR_MUST_EXIST );
    if( DirDialog )
    {
        if( DirDialog->ShowModal() == wxID_OK )
        {
            wxString NewFolder = DirDialog->GetPath() + wxT( "/" );
            if( NewFolder.StartsWith( MountPath ) )
            {
                NewFolder = NewFolder.Mid( MountPath.Length() );

                wxString PlaylistFolders = m_PlaylistFolderText->GetValue();

                if( PlaylistFolders.Find( NewFolder ) == wxNOT_FOUND )
                {
                    if( !PlaylistFolders.IsEmpty() )
                        PlaylistFolders.Append( wxT( ", " ) );

                    PlaylistFolders.Append( NewFolder );

                    m_PlaylistFolderText->SetValue( PlaylistFolders );
                }
            }
        }
        DirDialog->Destroy();
    }
}

// -------------------------------------------------------------------------------- //
void guPortableMediaProperties::OnPlaylistFormatBtnClick( wxCommandEvent &event )
{
    wxArrayInt Selection;
    wxArrayString Items;
    Items.Add( guPortablePlaylistFormatString[ 0 ] );
    Items.Add( guPortablePlaylistFormatString[ 1 ] );
    Items.Add( guPortablePlaylistFormatString[ 2 ] );
    Items.Add( guPortablePlaylistFormatString[ 3 ] );

    wxArrayInt ItemFlags;
    ItemFlags.Add( guPORTABLEMEDIA_PLAYLIST_FORMAT_M3U );
    ItemFlags.Add( guPORTABLEMEDIA_PLAYLIST_FORMAT_PLS );
    ItemFlags.Add( guPORTABLEMEDIA_PLAYLIST_FORMAT_XSPF );
    ItemFlags.Add( guPORTABLEMEDIA_PLAYLIST_FORMAT_ASX );

    int Index;
    int Count = ItemFlags.Count();
    for( Index = 0; Index < Count; Index++ )
    {
        if( m_PlaylistFormats & ItemFlags[ Index ] )
        {
            Selection.Add( Index );
        }
    }

    guListCheckOptionsDialog * ListCheckOptionsDialog = new guListCheckOptionsDialog( this, _( "Playlist format" ), Items, Selection );
    if( ListCheckOptionsDialog )
    {
        if( ListCheckOptionsDialog->ShowModal() == wxID_OK )
        {
            ListCheckOptionsDialog->GetSelectedItems( Selection );
            m_PlaylistFormats = 0;
            Count = Selection.Count();
            for( Index = 0; Index < Count; Index++ )
            {
                m_PlaylistFormats |= ItemFlags[ Selection[ Index ] ];
            }
            m_PlaylistFormatText->SetValue( m_PortableMediaDevice->PlaylistFormatsStr( m_PlaylistFormats ) );
        }
        ListCheckOptionsDialog->Destroy();
    }
}

// -------------------------------------------------------------------------------- //
void guPortableMediaProperties::OnCoverFormatBtnClick( wxCommandEvent &event )
{
    wxArrayInt Selection;
    wxArrayString Items;
    Items.Add( guPortableCoverFormatString[ 0 ] );
    Items.Add( guPortableCoverFormatString[ 1 ] );
    Items.Add( guPortableCoverFormatString[ 2 ] );
    Items.Add( guPortableCoverFormatString[ 3 ] );

    wxArrayInt ItemFlags;
    ItemFlags.Add( guPORTABLEMEDIA_COVER_FORMAT_JPEG );
    ItemFlags.Add( guPORTABLEMEDIA_COVER_FORMAT_PNG );
    ItemFlags.Add( guPORTABLEMEDIA_COVER_FORMAT_BMP );
    ItemFlags.Add( guPORTABLEMEDIA_COVER_FORMAT_GIF );

    int Index;
    int Count = ItemFlags.Count();
    for( Index = 0; Index < Count; Index++ )
    {
        if( m_CoverFormats & ItemFlags[ Index ] )
        {
            Selection.Add( Index );
        }
    }

    guListCheckOptionsDialog * ListCheckOptionsDialog = new guListCheckOptionsDialog( this, _( "Cover format" ), Items, Selection );
    if( ListCheckOptionsDialog )
    {
        if( ListCheckOptionsDialog->ShowModal() == wxID_OK )
        {
            ListCheckOptionsDialog->GetSelectedItems( Selection );
            m_CoverFormats = 0;
            Count = Selection.Count();
            for( Index = 0; Index < Count; Index++ )
            {
                m_CoverFormats |= ItemFlags[ Selection[ Index ] ];
            }
            m_CoverFormatText->SetValue( m_PortableMediaDevice->CoverFormatsStr( m_CoverFormats ) );
        }
        ListCheckOptionsDialog->Destroy();
    }
}

// -------------------------------------------------------------------------------- //
void guPortableMediaProperties::OnTransFormatChanged( wxCommandEvent& event )
{
    bool IsEnabled = event.GetInt() != guTRANSCODE_FORMAT_KEEP;

    m_TransScopeChoice->Enable( IsEnabled );
    m_TransQualityChoice->Enable( IsEnabled );
}

// -------------------------------------------------------------------------------- //
// guListCheckOptionsDialog
// -------------------------------------------------------------------------------- //
guListCheckOptionsDialog::guListCheckOptionsDialog( wxWindow * parent, const wxString &title,
    const wxArrayString &items, const wxArrayInt &selection ) :
    wxDialog( parent, wxID_ANY, title, wxDefaultPosition, wxSize( 225, 310 ), wxDEFAULT_DIALOG_STYLE )
{

	SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer * MainSizer = new wxBoxSizer( wxVERTICAL );

	m_CheckListBox = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, items, wxLB_MULTIPLE );
	int Index;
	int Count = selection.Count();
	for( Index = 0; Index < Count; Index++ )
	{
	    m_CheckListBox->Check( selection[ Index ] );
	}
	MainSizer->Add( m_CheckListBox, 1, wxALL|wxEXPAND, 5 );

    wxStdDialogButtonSizer * ButtonSizer;
    wxButton * ButtonSizerOK;
    wxButton * ButtonSizerCancel;
	ButtonSizer = new wxStdDialogButtonSizer();
	ButtonSizerOK = new wxButton( this, wxID_OK );
	ButtonSizer->AddButton( ButtonSizerOK );
	ButtonSizerCancel = new wxButton( this, wxID_CANCEL );
	ButtonSizer->AddButton( ButtonSizerCancel );
	ButtonSizer->Realize();
	MainSizer->Add( ButtonSizer, 0, wxEXPAND|wxALL, 5 );

	this->SetSizer( MainSizer );
	this->Layout();

}

// -------------------------------------------------------------------------------- //
guListCheckOptionsDialog::~guListCheckOptionsDialog()
{
}

// -------------------------------------------------------------------------------- //
int guListCheckOptionsDialog::GetSelectedItems( wxArrayInt &selection )
{
    selection.Empty();
    int Index;
    int Count = m_CheckListBox->GetCount();
    for( Index = 0; Index < Count; Index++ )
    {
        if( m_CheckListBox->IsChecked( Index ) )
            selection.Add( Index );
    }
    return selection.Count();
}

// -------------------------------------------------------------------------------- //