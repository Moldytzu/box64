#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "wrappedlibs.h"

#include "debug.h"
#include "wrapper.h"
#include "bridge.h"
#include "librarian/library_private.h"
#include "x64emu.h"
#include "emu/x64emu_private.h"
#include "callback.h"
#include "librarian.h"
#include "box64context.h"
#include "emu/x64emu_private.h"
#include "myalign.h"
#include "gtkclass.h"

const char* gtkx112Name = "libgtk-x11-2.0.so.0";
static char* libname = NULL;
#define LIBNAME gtkx112

typedef size_t        (*LFv_t)(void);
typedef void*         (*pFi_t)(int);
typedef void          (*vFp_t)(void*);
typedef int           (*iFp_t)(void*);
typedef void*         (*pFp_t)(void*);
typedef double        (*dFp_t)(void*);
typedef void*         (*pFL_t)(size_t);
typedef int           (*iFip_t)(int, void*);
typedef int           (*iFpp_t)(void*, void*);
typedef size_t        (*LFLp_t)(size_t, void*);
typedef void*         (*pFpp_t)(void*, void*);
typedef void*         (*pFpi_t)(void*, int);
typedef void*         (*pFpL_t)(void*, size_t);
typedef void          (*vFpp_t)(void*, void*);
typedef void*         (*pFppi_t)(void*, void*, int32_t);
typedef void          (*vFppA_t)(void*, void*, va_list);
typedef int32_t       (*iFppp_t)(void*, void*, void*);
typedef void          (*vFppp_t)(void*, void*, void*);
typedef uint32_t      (*uFupp_t)(uint32_t, void*, void*);
typedef void          (*vFpipV_t)(void*, int, void*, ...);
typedef int           (*iFpppp_t)(void*, void*, void*, void*);
typedef void          (*vFpppp_t)(void*, void*, void*, void*);
typedef void          (*vFppup_t)(void*, void*, uint32_t, void*);
typedef unsigned long (*LFppppi_t)(void*, void*, void*, void*, int);
typedef void          (*vFpippp_t)(void*, int, void*, void*, void*);
typedef void          (*vFppppp_t)(void*, void*, void*, void*, void*);
typedef void          (*vFpuipp_t)(void*, uint32_t, int, void*, void*);
typedef void          (*vFppupp_t)(void*, void*, uint32_t, void*, void*);
typedef uint32_t      (*uFiipppp_t)(int, int, void*, void*, void*, void*);
typedef unsigned long (*LFpppppi_t)(void*, void*, void*, void*, void*, int);
typedef int           (*iFpppppp_t)(void*, void*, void*, void*, void*, void*);
typedef void          (*vFpuipuV_t)(void*, uint32_t, int, void*, uint32_t, ...);
typedef int           (*iFppuppp_t)(void*, void*, uint32_t, void*, void*, void*);
typedef void*         (*pFppppppi_t)(void*, void*, void*, void*, void*, void*, int);
typedef void*         (*pFppppppp_t)(void*, void*, void*, void*, void*, void*, void*);
typedef void*         (*pFpppppppi_t)(void*, void*, void*, void*, void*, void*, void*, int);
typedef void          (*vFpppppuu_t)(void*, void*, void*, void*, void*, uint32_t, uint32_t);
typedef unsigned long (*LFppppppii_t)(void*, void*, void*, void*, void*, void*, int32_t, int32_t);
typedef void*         (*pFpippppppp_t)(void*, int, void*, void*, void*, void*, void*, void*, void*);
typedef void*         (*pFpipppppppi_t)(void*, int, void*, void*, void*, void*, void*, void*, void*, int);

#define SUPER() \
    GO(gtk_object_get_type, LFv_t)              \
    GO(gtk_object_set_data_full, vFpppp_t)      \
    GO(g_type_check_instance_cast, pFpL_t)      \
    GO(gtk_bin_get_type, LFv_t)                 \
    GO(gtk_widget_get_type, LFv_t)              \
    GO(gtk_button_get_type, LFv_t)              \
    GO(gtk_container_get_type, LFv_t)           \
    GO(gtk_misc_get_type, LFv_t)                \
    GO(gtk_label_get_type, LFv_t)               \
    GO(gtk_tree_view_get_type, LFv_t)           \
    GO(gtk_window_get_type, LFv_t)              \
    GO(gtk_table_get_type, LFv_t)               \
    GO(gtk_type_class, pFL_t)                   \
    GO(gtk_button_get_label, pFp_t)             \
    GO(gtk_signal_connect_full, LFppppppii_t)   \
    GO(gtk_dialog_add_button, pFppi_t)          \
    GO(gtk_message_dialog_format_secondary_text, vFpp_t)    \
    GO(gtk_message_dialog_format_secondary_markup, vFpp_t)  \
    GO(gtk_init, vFpp_t)                        \
    GO(gtk_init_check, iFpp_t)                  \
    GO(gtk_init_with_args, iFpppppp_t)          \
    GO(gtk_menu_attach_to_widget, vFppp_t)      \
    GO(gtk_menu_popup, vFpppppuu_t)             \
    GO(gtk_timeout_add, uFupp_t)                \
    GO(gtk_clipboard_set_with_data, iFppuppp_t) \
    GO(gtk_clipboard_set_with_owner, iFppuppp_t)\
    GO(gtk_stock_set_translate_func, vFpppp_t)  \
    GO(gtk_container_forall, vFppp_t)           \
    GO(gtk_tree_view_set_search_equal_func, vFpppp_t)   \
    GO(gtk_text_iter_backward_find_char, iFpppp_t)      \
    GO(gtk_text_iter_forward_find_char, iFpppp_t)       \
    GO(gtk_toolbar_append_item, pFppppppp_t)    \
    GO(gtk_toolbar_prepend_item, pFppppppp_t)   \
    GO(gtk_toolbar_insert_item, pFpppppppi_t)   \
    GO(gtk_toolbar_append_element, pFpippppppp_t)       \
    GO(gtk_toolbar_prepend_element, pFpippppppp_t)      \
    GO(gtk_toolbar_insert_element, pFpipppppppi_t)      \
    GO(gtk_toolbar_insert_stock, pFppppppi_t)   \
    GO(gtk_tree_sortable_set_sort_func, vFpippp_t)      \
    GO(gtk_tree_sortable_set_default_sort_func, vFpppp_t)\
    GO(gtk_type_unique, LFLp_t)                 \
    GO(gtk_spin_button_get_value, dFp_t)        \
    GO(gtk_builder_connect_signals_full, vFppp_t)       \
    GO(gtk_action_get_type, LFv_t)              \
    GO(gtk_binding_entry_add_signall, vFpuipp_t)\
    GO(gtk_binding_entry_add_signal, vFpuipuV_t)\
    GO(gtk_container_foreach, vFppp_t)          \
    GO(gtk_cell_layout_set_cell_data_func, vFppppp_t)   \
    GO(g_module_close, iFp_t)                   \
    GO(g_module_open, pFpi_t)                   \
    GO(g_module_supported, LFv_t)               \
    GO(g_module_symbol, iFppp_t)                \
    GO(g_log, vFpipV_t)                         \
    GO(g_type_class_ref, pFL_t)                 \
    GO(g_type_class_unref, vFp_t)               \
    GO(g_signal_connect_object, LFppppi_t)      \
    GO(g_signal_connect_data, LFpppppi_t)       \
    GO(gtk_action_group_add_actions, vFppup_t)  \
    GO(gtk_action_group_add_actions_full, vFppupp_t)    \
    GO(gtk_tree_model_foreach, vFppp_t)         \
    GO(gtk_clipboard_request_text, vFppp_t)     \
    GO(gtk_clipboard_request_contents, vFpppp_t)\
    GO(gtk_input_add_full, uFiipppp_t)          \
    GO(gtk_list_store_set_valist, vFppA_t)      \
    GO(gtk_widget_style_get_valist, vFppA_t)    \



typedef struct gtkx112_my_s {
    // functions
    #define GO(A, B)    B   A;
    SUPER()
    #undef GO
    int  gtk1;
} gtkx112_my_t;

void* getGtkx112My(library_t* lib)
{
    gtkx112_my_t* my = (gtkx112_my_t*)calloc(1, sizeof(gtkx112_my_t));
    #define GO(A, W) my->A = (W)dlsym(lib->priv.w.lib, #A);
    SUPER()
    #undef GO
    if(strcmp(lib->name, "libgtk-1.2.so.0")==0)
        my->gtk1 = 1;
    return my;
}
#undef SUPER

void freeGtkx112My(void* lib)
{
    //gtkx112_my_t *my = (gtkx112_my_t *)lib;
}

static box64context_t* context = NULL;

EXPORT uintptr_t my_gtk_signal_connect_full(x64emu_t* emu, void* object, void* name, void* c_handler, void* unsupported, void* data, void* closure, uint32_t signal, int after)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    if(!context)
        context = emu->context;

    my_signal_t *sig = new_mysignal(c_handler, data, closure);
    uintptr_t ret = my->gtk_signal_connect_full(object, name, my_signal_cb, NULL, sig, my_signal_delete, signal, after);
    printf_log(LOG_DEBUG, "Connecting gtk signal \"%s\" with cb=%p\n", (char*)name, sig);
    return ret;
}

// this is quite ineficient, but GCallback is often used, so create a large pool here...
#define SUPER() \
GO(0)   \
GO(1)   \
GO(2)   \
GO(3)   \
GO(4)   \
GO(5)   \
GO(6)   \
GO(7)   \
GO(8)   \
GO(9)   \
GO(10)  \
GO(11)  \
GO(12)  \
GO(13)  \
GO(14)  \
GO(15)  \
GO(16)  \
GO(17)  \
GO(18)  \
GO(19)  \
GO(20)  \
GO(21)  \
GO(22)  \
GO(23)  \
GO(24)  \
GO(25)  \
GO(26)  \
GO(27)  \
GO(28)  \
GO(29)  \
GO(30)  \
GO(31)  \
GO(32)  \
GO(33)  \
GO(34)  \
GO(35)  \
GO(36)  \
GO(37)  \
GO(38)  \
GO(39)  \

// GtkMenuDetachFunc
#define GO(A)   \
static uintptr_t my_menudetach_fct_##A = 0;   \
static void my_menudetach_##A(void* widget, void* menu)     \
{                                       \
    RunFunction(my_context, my_menudetach_fct_##A, 2, widget, menu);\
}
SUPER()
#undef GO
static void* findMenuDetachFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_menudetach_fct_##A == (uintptr_t)fct) return my_menudetach_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_menudetach_fct_##A == 0) {my_menudetach_fct_##A = (uintptr_t)fct; return my_menudetach_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 GMenuDetachFunc callback\n");
    return NULL;
}

// GtkMenuPositionFunc
#define GO(A)   \
static uintptr_t my_menuposition_fct_##A = 0;   \
static void my_menuposition_##A(void* menu, void* x, void* y, void* push_in, void* data)     \
{                                       \
    RunFunction(my_context, my_menuposition_fct_##A, 5, menu, x, y, push_in, data);\
}
SUPER()
#undef GO
static void* findMenuPositionFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_menuposition_fct_##A == (uintptr_t)fct) return my_menuposition_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_menuposition_fct_##A == 0) {my_menuposition_fct_##A = (uintptr_t)fct; return my_menuposition_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 GtkMenuPositionFunc callback\n");
    return NULL;
}

// GtkFunction
#define GO(A)   \
static uintptr_t my_gtkfunction_fct_##A = 0;   \
static int my_gtkfunction_##A(void* data)     \
{                                       \
    return RunFunction(my_context, my_gtkfunction_fct_##A, 1, data);\
}
SUPER()
#undef GO
static void* findGtkFunctionFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_gtkfunction_fct_##A == (uintptr_t)fct) return my_gtkfunction_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_gtkfunction_fct_##A == 0) {my_gtkfunction_fct_##A = (uintptr_t)fct; return my_gtkfunction_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 GtkFunction callback\n");
    return NULL;
}

// GtkClipboardGetFunc
#define GO(A)   \
static uintptr_t my_clipboardget_fct_##A = 0;   \
static void my_clipboardget_##A(void* clipboard, void* selection, uint32_t info, void* data)     \
{                                       \
    RunFunction(my_context, my_clipboardget_fct_##A, 4, clipboard, selection, info, data);\
}
SUPER()
#undef GO
static void* findClipboadGetFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_clipboardget_fct_##A == (uintptr_t)fct) return my_clipboardget_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_clipboardget_fct_##A == 0) {my_clipboardget_fct_##A = (uintptr_t)fct; return my_clipboardget_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 GtkClipboardGetFunc callback\n");
    return NULL;
}

// GtkClipboardClearFunc
#define GO(A)   \
static uintptr_t my_clipboardclear_fct_##A = 0;   \
static void my_clipboardclear_##A(void* clipboard, void* data)     \
{                                       \
    RunFunction(my_context, my_clipboardclear_fct_##A, 2, clipboard, data);\
}
SUPER()
#undef GO
static void* findClipboadClearFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_clipboardclear_fct_##A == (uintptr_t)fct) return my_clipboardclear_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_clipboardclear_fct_##A == 0) {my_clipboardclear_fct_##A = (uintptr_t)fct; return my_clipboardclear_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 GtkClipboardClearFunc callback\n");
    return NULL;
}

// GtkCallback
#define GO(A)   \
static uintptr_t my_gtkcallback_fct_##A = 0;   \
static void my_gtkcallback_##A(void* widget, void* data)     \
{                                       \
    RunFunction(my_context, my_gtkcallback_fct_##A, 2, widget, data);\
}
SUPER()
#undef GO
static void* findGtkCallbackFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_gtkcallback_fct_##A == (uintptr_t)fct) return my_gtkcallback_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_gtkcallback_fct_##A == 0) {my_gtkcallback_fct_##A = (uintptr_t)fct; return my_gtkcallback_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 GtkCallback callback\n");
    return NULL;
}

// GtkTextCharPredicate
#define GO(A)   \
static uintptr_t my_textcharpredicate_fct_##A = 0;   \
static int my_textcharpredicate_##A(uint32_t ch, void* data)     \
{                                       \
    return (int)RunFunction(my_context, my_textcharpredicate_fct_##A, 2, ch, data);\
}
SUPER()
#undef GO
static void* findGtkTextCharPredicateFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_textcharpredicate_fct_##A == (uintptr_t)fct) return my_textcharpredicate_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_textcharpredicate_fct_##A == 0) {my_textcharpredicate_fct_##A = (uintptr_t)fct; return my_textcharpredicate_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 GtkTextCharPredicate callback\n");
    return NULL;
}

// Toolbar
#define GO(A)   \
static uintptr_t my_toolbar_fct_##A = 0;   \
static void my_toolbar_##A(void* widget, void* data)     \
{                                       \
    RunFunction(my_context, my_toolbar_fct_##A, 2, widget, data);\
}
SUPER()
#undef GO
static void* findToolbarFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_toolbar_fct_##A == (uintptr_t)fct) return my_toolbar_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_toolbar_fct_##A == 0) {my_toolbar_fct_##A = (uintptr_t)fct; return my_toolbar_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 Toolbar callback\n");
    return NULL;
}

// Builder
#define GO(A)   \
static uintptr_t my_builderconnect_fct_##A = 0;   \
static void my_builderconnect_##A(void* builder, void* object, void* signal, void* handler, void* connect, int flags, void* data)     \
{                                       \
    RunFunction(my_context, my_builderconnect_fct_##A, 7, builder, object, signal, handler, connect, flags, data);\
}
SUPER()
#undef GO
static void* findBuilderConnectFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_builderconnect_fct_##A == (uintptr_t)fct) return my_builderconnect_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_builderconnect_fct_##A == 0) {my_builderconnect_fct_##A = (uintptr_t)fct; return my_builderconnect_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 BuilderConnect callback\n");
    return NULL;
}

// GtkCellLayoutDataFunc
#define GO(A)   \
static uintptr_t my_GtkCellLayoutDataFunc_fct_##A = 0;                                                  \
static void my_GtkCellLayoutDataFunc_##A(void* layout, void* cell, void* tree, void* iter, void* data)  \
{                                       \
    RunFunction(my_context, my_GtkCellLayoutDataFunc_fct_##A, 5, layout, cell, tree, iter, data);\
}
SUPER()
#undef GO
static void* findGtkCellLayoutDataFuncFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_GtkCellLayoutDataFunc_fct_##A == (uintptr_t)fct) return my_GtkCellLayoutDataFunc_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_GtkCellLayoutDataFunc_fct_##A == 0) {my_GtkCellLayoutDataFunc_fct_##A = (uintptr_t)fct; return my_GtkCellLayoutDataFunc_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 GtkCellLayoutDataFunc callback\n");
    return NULL;
}

// GDestroyNotify
#define GO(A)   \
static uintptr_t my_GDestroyNotify_fct_##A = 0;                     \
static void my_GDestroyNotify_##A(void* data)                       \
{                                                                   \
    RunFunction(my_context, my_GDestroyNotify_fct_##A, 1, data);    \
}
SUPER()
#undef GO
static void* findGDestroyNotifyFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_GDestroyNotify_fct_##A == (uintptr_t)fct) return my_GDestroyNotify_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_GDestroyNotify_fct_##A == 0) {my_GDestroyNotify_fct_##A = (uintptr_t)fct; return my_GDestroyNotify_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 GDestroyNotify callback\n");
    return NULL;
}

// GtkTreeModelForeachFunc
#define GO(A)   \
static uintptr_t my_GtkTreeModelForeachFunc_fct_##A = 0;                                                    \
static int my_GtkTreeModelForeachFunc_##A(void* model, void* path, void* iter, void* data)                  \
{                                                                                                           \
    return (int)RunFunction(my_context, my_GtkTreeModelForeachFunc_fct_##A, 4, model, path, iter, data);    \
}
SUPER()
#undef GO
static void* findGtkTreeModelForeachFuncFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_GtkTreeModelForeachFunc_fct_##A == (uintptr_t)fct) return my_GtkTreeModelForeachFunc_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_GtkTreeModelForeachFunc_fct_##A == 0) {my_GtkTreeModelForeachFunc_fct_##A = (uintptr_t)fct; return my_GtkTreeModelForeachFunc_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 GtkTreeModelForeachFunc callback\n");
    return NULL;
}

// GtkClipboardReceivedFunc
#define GO(A)   \
static uintptr_t my_GtkClipboardReceivedFunc_fct_##A = 0;                                   \
static void my_GtkClipboardReceivedFunc_##A(void* clipboard, void* sel, void* data)        \
{                                                                                               \
    RunFunction(my_context, my_GtkClipboardReceivedFunc_fct_##A, 3, clipboard, sel, data); \
}
SUPER()
#undef GO
static void* findGtkClipboardReceivedFuncFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_GtkClipboardReceivedFunc_fct_##A == (uintptr_t)fct) return my_GtkClipboardReceivedFunc_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_GtkClipboardReceivedFunc_fct_##A == 0) {my_GtkClipboardReceivedFunc_fct_##A = (uintptr_t)fct; return my_GtkClipboardReceivedFunc_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 GtkClipboardReceivedFunc callback\n");
    return NULL;
}

// GtkClipboardTextReceivedFunc
#define GO(A)   \
static uintptr_t my_GtkClipboardTextReceivedFunc_fct_##A = 0;                                   \
static void my_GtkClipboardTextReceivedFunc_##A(void* clipboard, void* text, void* data)        \
{                                                                                               \
    RunFunction(my_context, my_GtkClipboardTextReceivedFunc_fct_##A, 3, clipboard, text, data); \
}
SUPER()
#undef GO
static void* findGtkClipboardTextReceivedFuncFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_GtkClipboardTextReceivedFunc_fct_##A == (uintptr_t)fct) return my_GtkClipboardTextReceivedFunc_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_GtkClipboardTextReceivedFunc_fct_##A == 0) {my_GtkClipboardTextReceivedFunc_fct_##A = (uintptr_t)fct; return my_GtkClipboardTextReceivedFunc_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 GtkClipboardTextReceivedFunc callback\n");
    return NULL;
}

// GtkTreeViewSearchEqualFunc
#define GO(A)   \
static uintptr_t my_GtkTreeViewSearchEqualFunc_fct_##A = 0;                                                     \
static int my_GtkTreeViewSearchEqualFunc_##A(void* model, int column, void* key, void* iter, void* data)        \
{                                                                                                               \
    return RunFunction(my_context, my_GtkTreeViewSearchEqualFunc_fct_##A, 5, model, column, key, iter, data);   \
}
SUPER()
#undef GO
static void* findGtkTreeViewSearchEqualFuncFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_GtkTreeViewSearchEqualFunc_fct_##A == (uintptr_t)fct) return my_GtkTreeViewSearchEqualFunc_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_GtkTreeViewSearchEqualFunc_fct_##A == 0) {my_GtkTreeViewSearchEqualFunc_fct_##A = (uintptr_t)fct; return my_GtkTreeViewSearchEqualFunc_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 GtkTreeViewSearchEqualFunc callback\n");
    return NULL;
}

// GtkTreeIterCompareFunc
#define GO(A)   \
static uintptr_t my_GtkTreeIterCompareFunc_fct_##A = 0;                                         \
static int my_GtkTreeIterCompareFunc_##A(void* model, void* a, void* b, void* data)             \
{                                                                                               \
    return RunFunction(my_context, my_GtkTreeIterCompareFunc_fct_##A, 4, model, a, b, data);    \
}
SUPER()
#undef GO
static void* findGtkTreeIterCompareFuncFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_GtkTreeIterCompareFunc_fct_##A == (uintptr_t)fct) return my_GtkTreeIterCompareFunc_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_GtkTreeIterCompareFunc_fct_##A == 0) {my_GtkTreeIterCompareFunc_fct_##A = (uintptr_t)fct; return my_GtkTreeIterCompareFunc_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 GtkTreeIterCompareFunc callback\n");
    return NULL;
}

// GdkInputFunction
#define GO(A)   \
static uintptr_t my_GdkInputFunction_fct_##A = 0;                                  \
static void my_GdkInputFunction_##A(void* data, int source, int cond)              \
{                                                                                   \
    RunFunction(my_context, my_GdkInputFunction_fct_##A, 3, data, source, cond);   \
}
SUPER()
#undef GO
static void* findGdkInputFunctionFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_GdkInputFunction_fct_##A == (uintptr_t)fct) return my_GdkInputFunction_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_GdkInputFunction_fct_##A == 0) {my_GdkInputFunction_fct_##A = (uintptr_t)fct; return my_GdkInputFunction_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 GdkInputFunction callback\n");
    return NULL;
}

// GtkCallbackMarshal
#define GO(A)   \
static uintptr_t my_GtkCallbackMarshal_fct_##A = 0;                                 \
static void my_GtkCallbackMarshal_##A(void* obj, void* data, uint32_t n, void* args)\
{                                                                                   \
    RunFunction(my_context, my_GtkCallbackMarshal_fct_##A, 4, obj, data, n, args);  \
}
SUPER()
#undef GO
static void* findGtkCallbackMarshalFct(void* fct)
{
    if(!fct) return fct;
    if(GetNativeFnc((uintptr_t)fct))  return GetNativeFnc((uintptr_t)fct);
    #define GO(A) if(my_GtkCallbackMarshal_fct_##A == (uintptr_t)fct) return my_GtkCallbackMarshal_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_GtkCallbackMarshal_fct_##A == 0) {my_GtkCallbackMarshal_fct_##A = (uintptr_t)fct; return my_GtkCallbackMarshal_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for gtk-2 GtkCallbackMarshal callback\n");
    return NULL;
}

#undef SUPER

EXPORT void my_gtk_dialog_add_buttons(x64emu_t* emu, void* dialog, void* first, uintptr_t* b)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    void* btn = first;
    while(btn) {
        uintptr_t id = *(b++);
        my->gtk_dialog_add_button(dialog, btn, id);
        btn = (void*)*(b++);
    }
}

EXPORT void my_gtk_message_dialog_format_secondary_text(x64emu_t* emu, void* dialog, void* fmt, void* b)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    char* buf = NULL;
    myStackAlign(emu, (const char*)fmt, b, emu->scratch, R_EAX, 2);
    PREPARE_VALIST;
    int ret = vasprintf(&buf, fmt, VARARGS);
    (void)ret;
    // pre-bake the fmt/vaarg, because there is no "va_list" version of this function
    my->gtk_message_dialog_format_secondary_text(dialog, buf);
    free(buf);
}

EXPORT void my_gtk_message_dialog_format_secondary_markup(x64emu_t* emu, void* dialog, void* fmt, void* b)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    char* buf = NULL;
    myStackAlign(emu, (const char*)fmt, b, emu->scratch, R_EAX, 2);
    PREPARE_VALIST;
    int ret = vasprintf(&buf, fmt, VARARGS);
    (void)ret;
    // pre-bake the fmt/vaarg, because there is no "va_list" version of this function
    my->gtk_message_dialog_format_secondary_markup(dialog, buf);
    free(buf);
}

EXPORT void my_gtk_list_store_set_valist(x64emu_t* emu, void* list, void* iter, x64_va_list_t V)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    CONVERT_VALIST(V);
    my->gtk_list_store_set_valist(list, iter, VARARGS);
}

EXPORT void my_gtk_list_store_set(x64emu_t* emu, void* list, void* iter, uintptr_t* b)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    CREATE_VALIST_FROM_VAARG(b, emu->scratch, 2);
    my->gtk_list_store_set_valist(list, iter, VARARGS);
}

EXPORT void my_gtk_widget_style_get_valist(x64emu_t* emu, void* widget, void* first, x64_va_list_t V)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    CONVERT_VALIST(V);
    my->gtk_widget_style_get_valist(widget, first, VARARGS);
}

EXPORT void my_gtk_widget_style_get(x64emu_t* emu, void* widget, void* first, uintptr_t* b)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    CREATE_VALIST_FROM_VAARG(b, emu->scratch, 2);
    my->gtk_widget_style_get_valist(widget, first, VARARGS);
}

EXPORT void* my_gtk_type_class(x64emu_t* emu, size_t type)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    void* class = my->gtk_type_class(type);
    return wrapCopyGTKClass(class, type);
}

EXPORT void my_gtk_init(x64emu_t* emu, void* argc, void* argv)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my->gtk_init(argc, argv);
    my_checkGlobalGdkDisplay();
    AutoBridgeGtk(my->g_type_class_ref, my->g_type_class_unref);
}

EXPORT int my_gtk_init_check(x64emu_t* emu, void* argc, void* argv)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    int ret = my->gtk_init_check(argc, argv);
    my_checkGlobalGdkDisplay();
    AutoBridgeGtk(my->g_type_class_ref, my->g_type_class_unref);
    return ret;
}

EXPORT int my_gtk_init_with_args(x64emu_t* emu, void* argc, void* argv, void* param, void* entries, void* trans, void* error)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    int ret = my->gtk_init_with_args(argc, argv, param, entries, trans, error);
    my_checkGlobalGdkDisplay();
    AutoBridgeGtk(my->g_type_class_ref, my->g_type_class_unref);
    return ret;
}

EXPORT void my_gtk_menu_attach_to_widget(x64emu_t* emu, void* menu, void* widget, void* f)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my->gtk_menu_attach_to_widget(menu, widget, findMenuDetachFct(f));
}

EXPORT void my_gtk_menu_popup(x64emu_t* emu, void* menu, void* shell, void* item, void* f, void* data, uint32_t button, uint32_t time_)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my->gtk_menu_popup(menu, shell, item, findMenuPositionFct(f), data, button, time_);
}

EXPORT uint32_t my_gtk_timeout_add(x64emu_t* emu, uint32_t interval, void* f, void* data)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    return my->gtk_timeout_add(interval, findGtkFunctionFct(f), data);
}

EXPORT int my_gtk_clipboard_set_with_data(x64emu_t* emu, void* clipboard, void* target, uint32_t n, void* f_get, void* f_clear, void* data)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    return my->gtk_clipboard_set_with_data(clipboard, target, n, findClipboadGetFct(f_get), findClipboadClearFct(f_clear), data);
}

EXPORT int my_gtk_clipboard_set_with_owner(x64emu_t* emu, void* clipboard, void* target, uint32_t n, void* f_get, void* f_clear, void* data)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    return my->gtk_clipboard_set_with_owner(clipboard, target, n, findClipboadGetFct(f_get), findClipboadClearFct(f_clear), data);
}

static void* my_translate_func(void* path, my_signal_t* sig)
{
    return (void*)RunFunction(my_context, sig->c_handler, 2, path, sig->data);
}

EXPORT void my_gtk_stock_set_translate_func(x64emu_t* emu, void* domain, void* f, void* data, void* notify)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my_signal_t *sig = new_mysignal(f, data, notify);
    my->gtk_stock_set_translate_func(domain, my_translate_func, sig, my_signal_delete);
}

EXPORT void my_gtk_container_forall(x64emu_t* emu, void* container, void* f, void* data)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my->gtk_container_forall(container, findGtkCallbackFct(f), data);
}

EXPORT void my_gtk_tree_view_set_search_equal_func(x64emu_t* emu, void* tree_view, void* f, void* data, void* notify)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my->gtk_tree_view_set_search_equal_func(tree_view, findGtkTreeViewSearchEqualFuncFct(f), data, findGDestroyNotifyFct(notify));
}

EXPORT int my_gtk_text_iter_backward_find_char(x64emu_t* emu, void* iter, void* f, void* data, void* limit)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    return my->gtk_text_iter_backward_find_char(iter, findGtkTextCharPredicateFct(f), data, limit);
}

EXPORT int my_gtk_text_iter_forward_find_char(x64emu_t* emu, void* iter, void* f, void* data, void* limit)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    return my->gtk_text_iter_forward_find_char(iter, findGtkTextCharPredicateFct(f), data, limit);
}

EXPORT void* my_gtk_toolbar_append_item(x64emu_t* emu, void* toolbar, void* text, void* tooltip_text, void* tooltip_private, void* icon, void* f, void* data)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    return my->gtk_toolbar_append_item(toolbar, text, tooltip_text, tooltip_private, icon, findToolbarFct(f), data);
}

EXPORT void* my_gtk_toolbar_prepend_item(x64emu_t* emu, void* toolbar, void* text, void* tooltip_text, void* tooltip_private, void* icon, void* f, void* data)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    return my->gtk_toolbar_prepend_item(toolbar, text, tooltip_text, tooltip_private, icon, findToolbarFct(f), data);
}

EXPORT void* my_gtk_toolbar_insert_item(x64emu_t* emu, void* toolbar, void* text, void* tooltip_text, void* tooltip_private, void* icon, void* f, void* data, int position)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    return my->gtk_toolbar_insert_item(toolbar, text, tooltip_text, tooltip_private, icon, findToolbarFct(f), data, position);
}

EXPORT void* my_gtk_toolbar_append_element(x64emu_t* emu, void* toolbar, size_t type, void* widget, void* text, void* tooltip_text, void* tooltip_private, void* icon, void* f, void* data)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    return my->gtk_toolbar_append_element(toolbar, type, widget, text, tooltip_text, tooltip_private, icon, findToolbarFct(f), data);
}

EXPORT void* my_gtk_toolbar_prepend_element(x64emu_t* emu, void* toolbar, size_t type, void* widget, void* text, void* tooltip_text, void* tooltip_private, void* icon, void* f, void* data)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    return my->gtk_toolbar_prepend_element(toolbar, type, widget, text, tooltip_text, tooltip_private, icon, findToolbarFct(f), data);
}

EXPORT void* my_gtk_toolbar_insert_element(x64emu_t* emu, void* toolbar, size_t type, void* widget, void* text, void* tooltip_text, void* tooltip_private, void* icon, void* f, void* data, int position)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    return my->gtk_toolbar_insert_element(toolbar, type, widget, text, tooltip_text, tooltip_private, icon, findToolbarFct(f), data, position);
}

EXPORT void* my_gtk_toolbar_insert_stock(x64emu_t* emu, void* toolbar, void* stock_id, void* tooltip_text, void* tooltip_private, void* f, void* data, int position)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    return my->gtk_toolbar_insert_stock(toolbar, stock_id, tooltip_text, tooltip_private, findToolbarFct(f), data, position);
}

EXPORT void my_gtk_tree_sortable_set_sort_func(x64emu_t* emu, void* sortable, int id, void* f, void* data, void* notify)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my->gtk_tree_sortable_set_sort_func(sortable, id, findGtkTreeIterCompareFuncFct(f), data, findGDestroyNotifyFct(notify));
}

EXPORT void my_gtk_tree_sortable_set_default_sort_func(x64emu_t* emu, void* sortable, void* f, void* data, void* notify)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my->gtk_tree_sortable_set_default_sort_func(sortable, findGtkTreeIterCompareFuncFct(f), data, findGDestroyNotifyFct(notify));
}

EXPORT size_t my_gtk_type_unique(x64emu_t* emu, size_t parent, my_GtkTypeInfo_t* gtkinfo)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    return my->gtk_type_unique(parent, findFreeGtkTypeInfo(gtkinfo, parent));
}

EXPORT unsigned long my_gtk_signal_connect(x64emu_t* emu, void* object, void* name, void* func, void* data)
{
    return my_gtk_signal_connect_full(emu, object, name, func, NULL, data, NULL, 0, 0);
}

EXPORT void my_gtk_object_set_data_full(x64emu_t* emu, void* object, void* key, void* data, void* notify)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my->gtk_object_set_data_full(object, key, data, findGDestroyNotifyFct(notify));
}

EXPORT float my_gtk_spin_button_get_value_as_float(x64emu_t* emu, void* spinner)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    return my->gtk_spin_button_get_value(spinner);
}

static size_t gtk1Type(gtkx112_my_t *my, size_t type)
{
    if (type==21)
        return my->gtk_object_get_type();
    return type;    // other changes needed?
}

typedef struct dummy_gtk1_button_s {
    size_t type;
    int dummy[14];
    void* label;
} dummy_gtk1_button_t;

EXPORT void* my_gtk_type_check_object_cast(x64emu_t* emu, void* obj, size_t type)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    if(my->gtk1 && type==my->gtk_bin_get_type())
    {
        // check if obj is a button...
        if(my->g_type_check_instance_cast(obj, my->gtk_button_get_type()))
        {
            // make some bad casting of button from gtkv2 to button from gtkv1, the "bin" sized changed, and is shorter now!
            // shogo launcher acces label from button directly from the cast (+0x3c)
            static dummy_gtk1_button_t dummy = {0};
            dummy.type = my->gtk_button_get_type();
            void **p = my->g_type_check_instance_cast(obj, my->gtk_bin_get_type());
            // style is 0x06
            dummy.label = p[0x11];
            return &dummy;
        }
    }

    return my->g_type_check_instance_cast(obj, gtk1Type(my, type));
}

EXPORT void my_gtk_builder_connect_signals_full(x64emu_t* emu, void* builder, void* f, void* data)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my->gtk_builder_connect_signals_full(builder, findBuilderConnectFct(f), data);
}

typedef struct my_GSList_s {
  void*               data;
  struct my_GSList_s *next;
} my_GSList_t;

static int regs_abi[] = {_DI, _SI, _DX, _CX, _R8, _R9};
#define NEXT5(A, N) (N+5<6)?((void*)emu->regs[regs_abi[5+N]].q[0]):A[5+N-6]
EXPORT void my_gtk_binding_entry_add_signal(x64emu_t* emu, void* binding, uint32_t keyval, int mod, void* name, uint32_t n, void** st)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;
    if(!n) {
        my->gtk_binding_entry_add_signal(binding, keyval, mod, name, 0);
        return;
    }
    // build the list
    my_GSList_t *list = calloc(n, sizeof(my_GSList_t));
    for(uint32_t i=0; i<n; ++i) {
        list[i].data = NEXT5(st, i);
        list[i].next = (i==(n-1))?NULL:&list[i+1];
    }

    my->gtk_binding_entry_add_signall(binding, keyval, mod, name, list);

    free(list);
}

EXPORT void my_gtk_container_foreach(x64emu_t* emu, void* container, void* f, void* data)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my->gtk_container_foreach(container, findGtkCallbackFct(f), data);
}

EXPORT void my_gtk_cell_layout_set_cell_data_func(x64emu_t* emu, void* layout, void* cell, void* f, void* data, void* notify)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my->gtk_cell_layout_set_cell_data_func(layout, cell, findGtkCellLayoutDataFuncFct(f), data, findGDestroyNotifyFct(notify));
}

typedef struct my_ConnectArgs_s
{
    gtkx112_my_t *my;
    x64emu_t* emu;
    void* module;
    void* data;
} my_ConnectArgs_t;
// defined in wrappedgobject2.c
uintptr_t my_g_signal_connect_data(x64emu_t* emu, void* instance, void* detailed, void* c_handler, void* data, void* closure, uint32_t flags);

static void my_gtk_builder_connect_signals_custom(void* builder,
                                                  void* object,
                                                  char* signal_name,
                                                  char* handler_name,
                                                  void* connect_object,
                                                  int   flags,
                                                  my_ConnectArgs_t* args)
{
    /* Only error out for missing GModule support if we've not
    * found the symbols explicitly added with gtk_builder_add_callback_symbol()
    */
    void* func = NULL;
    printf_log(LOG_DEBUG, "signal \"%s\" from \"%s\" connection, connect_object=%p\n", signal_name, handler_name, connect_object);

    uintptr_t offs = 0;
    uintptr_t end = 0;
    GetGlobalSymbolStartEnd(my_context->maplib, handler_name, &offs, &end, NULL, -1, NULL);
    if(!offs) {
        if (args->module == NULL)
            args->my->g_log("Gtk", 1<<2 ,"gtk_builder_connect_signals() requires working GModule");
        if (!args->my->g_module_symbol(args->module, handler_name, (void*)&func))
        {
            args->my->g_log("Gtk", 1<<4, "Could not find signal handler '%s'.  Did you compile with -rdynamic?", handler_name);
            return;
        }
        if (connect_object)
            args->my->g_signal_connect_object(object, signal_name, func, connect_object, flags);
        else
            args->my->g_signal_connect_data(object, signal_name, func, args->data, NULL, flags);
    } else {
        if(connect_object) {
            printf_log(LOG_NONE, "Error: connect custom signal to object unsupported\n");
        } else
            my_g_signal_connect_data(args->emu, object, signal_name, (void*)offs, args->data, NULL, flags);
    }
}

EXPORT void my_gtk_builder_connect_signals(x64emu_t* emu, void* builder, void* data)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my_ConnectArgs_t args;

    args.my = my;
    args.emu = emu;
    args.data = data;
    if (my->g_module_supported())
        args.module = my->g_module_open(NULL, 1);
    my->gtk_builder_connect_signals_full(builder, my_gtk_builder_connect_signals_custom, &args);
    if (args.module)
        my->g_module_close(args.module);
}

typedef struct my_GtkActionEntry_s {
  const char* name;
  const char* stock_id;
  const char* label;
  const char* accelerator;
  const char* tooltip;
  void*       callback;
} my_GtkActionEntry_t;

EXPORT void my_gtk_action_group_add_actions(x64emu_t* emu, void* action_group, my_GtkActionEntry_t* entries, uint32_t n, void* data)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my_GtkActionEntry_t myentries[n];
    memcpy(myentries, entries, n*sizeof(my_GtkActionEntry_t));
    for(uint32_t i=0; i<n; ++i)
        myentries[i].callback = findGtkCallbackFct(entries[i].callback);
    my->gtk_action_group_add_actions(action_group, myentries, n, data);
}

EXPORT void my_gtk_action_group_add_actions_full(x64emu_t* emu, void* action_group, my_GtkActionEntry_t* entries, uint32_t n, void* data, void* destroy)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my_GtkActionEntry_t myentries[n];
    memcpy(myentries, entries, n*sizeof(my_GtkActionEntry_t));
    for(uint32_t i=0; i<n; ++i)
        myentries[i].callback = findGtkCallbackFct(entries[i].callback);
    my->gtk_action_group_add_actions_full(action_group, myentries, n, data, findGDestroyNotifyFct(destroy));
}

EXPORT void my_gtk_tree_model_foreach(x64emu_t* emu, void* model, void* f, void* data)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my->gtk_tree_model_foreach(model, findGtkTreeModelForeachFuncFct(f), data);
}

EXPORT void my_gtk_clipboard_request_contents(x64emu_t* emu, void* clipboard, void* target, void* f, void* data)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my->gtk_clipboard_request_contents(clipboard, target, findGtkClipboardReceivedFuncFct(f), data);
}


EXPORT void my_gtk_clipboard_request_text(x64emu_t* emu, void* clipboard, void* f, void* data)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    my->gtk_clipboard_request_text(clipboard, findGtkClipboardTextReceivedFuncFct(f), data);
}

EXPORT uint32_t my_gtk_input_add_full(x64emu_t* emu, int source, int condition, void* func, void* marshal, void* data, void* destroy)
{
    library_t * lib = GetLibInternal(libname);
    gtkx112_my_t *my = (gtkx112_my_t*)lib->priv.w.p2;

    return my->gtk_input_add_full(source, condition, findGdkInputFunctionFct(func), findGtkCallbackMarshalFct(marshal), data, findGDestroyNotifyFct(destroy));
}

#define PRE_INIT    \
    if(box64_nogtk) \
        return -1;

#define CUSTOM_INIT \
    libname = lib->name;                \
    lib->priv.w.p2 = getGtkx112My(lib); \
    SetGtkObjectID(((gtkx112_my_t*)lib->priv.w.p2)->gtk_object_get_type());     \
    SetGtkWidgetID(((gtkx112_my_t*)lib->priv.w.p2)->gtk_widget_get_type());     \
    SetGtkContainerID(((gtkx112_my_t*)lib->priv.w.p2)->gtk_container_get_type());\
    SetGtkActionID(((gtkx112_my_t*)lib->priv.w.p2)->gtk_action_get_type());     \
    SetGtkMiscID(((gtkx112_my_t*)lib->priv.w.p2)->gtk_misc_get_type());         \
    SetGtkLabelID(((gtkx112_my_t*)lib->priv.w.p2)->gtk_label_get_type());       \
    SetGtkTreeViewID(((gtkx112_my_t*)lib->priv.w.p2)->gtk_tree_view_get_type());\
    SetGtkBinID(((gtkx112_my_t*)lib->priv.w.p2)->gtk_bin_get_type());           \
    SetGtkWindowID(((gtkx112_my_t*)lib->priv.w.p2)->gtk_window_get_type());     \
    SetGtkTableID(((gtkx112_my_t*)lib->priv.w.p2)->gtk_table_get_type());     \
    lib->priv.w.needed = 2;                                                     \
    lib->priv.w.neededlibs = (char**)calloc(lib->priv.w.needed, sizeof(char*)); \
    lib->priv.w.neededlibs[0] = strdup("libgdk-x11-2.0.so.0");                  \
    lib->priv.w.neededlibs[1] = strdup("libpangocairo-1.0.so.0");

#define CUSTOM_FINI \
    freeGtkx112My(lib->priv.w.p2); \
    free(lib->priv.w.p2);

#include "wrappedlib_init.h"