#include "hooks.hpp"
#include "base_cheat.hpp"
#include "context.hpp"

void __fastcall hooks::override_mouse_input( void* ecx_, void* edx_, float* x, float* y ) {
	static auto omi_o = override_mouse_input_o;
	omi_o( ecx_, edx_, x, y );

	if( g_csgo.m_panic ) return;

	if( g_ctx.run_frame( ) && g_ctx.m_local->is_alive( ) ) {
		g_cheat.m_legitbot( x, y );
	}
}