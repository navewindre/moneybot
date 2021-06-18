#include "hooks.h"
#include "settings.h"
#include "base_cheat.h"
#include "ctx.hpp"

void __fastcall hooks::scene_end( void* ecx_, void* edx_ ) {
	static auto scene_end_o = cl.m_render_view->get_old_function< decltype( &hooks::scene_end ) >( 9 );
	scene_end_o( edx_, 0 );

	if( !cl.m_panic && g_ctx.m_local && g_settings.visuals.chams.enabled( ) && g_settings.visuals.chams.ignore_z( ) ) {
		g_ctx.m_drawing_sceneend = true;
		for( int i = 1; i < cl.m_globals->maxclients; ++i ) {
			auto ent = cl.m_entlist( )->get_client_entity< c_base_player >( i );
			if( ent && ent->is_valid( ) ) {
				if( ent == g_ctx.m_local && ent->m_bIsScoped( ) ) {
					continue;
				}
				int team = ent->m_iTeamNum( );
				bool enemy = team != g_ctx.m_local->m_iTeamNum( );
				if( enemy || g_settings.visuals.chams.friendlies( ) ) {


					IMaterial* mat = g_settings.visuals.chams.flat ? g_cheat.chams.m_materials.m_chams_flat : g_cheat.chams.m_materials.m_chams;
					if( !mat ) {
						g_cheat.chams.m_materials.initialize_materials( );
						return;
					}

					fclr_t clr_vis = enemy ? g_settings.visuals.chams.color_visible_enemy( ).to_fclr( ) : g_settings.visuals.chams.color_visible_friendly( ).to_fclr( );
					fclr_t clr_hid = enemy ? g_settings.visuals.chams.color_hidden_enemy( ).to_fclr( ) : g_settings.visuals.chams.color_hidden_friendly( ).to_fclr( );
					mat->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );
					g_cheat.chams.m_materials.force_material( mat, clr_hid );
					ent->ce( )->DrawModel( 0x1, 255 );
					mat->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );
					g_cheat.chams.m_materials.force_material( mat, clr_vis );
					ent->ce( )->DrawModel( 0x1, 255 );
					cl.m_model_render( )->ForcedMaterialOverride( nullptr );
				}
			}
		}

		g_ctx.m_drawing_sceneend = false;
	}
}