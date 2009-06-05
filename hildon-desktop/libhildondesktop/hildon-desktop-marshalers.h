
#ifndef __g_cclosure_user_marshal_MARSHAL_H__
#define __g_cclosure_user_marshal_MARSHAL_H__

#include	<glib-object.h>

G_BEGIN_DECLS

/* OBJECT:OBJECT (hildon-desktop-marshalers.list:1) */
extern void g_cclosure_user_marshal_OBJECT__OBJECT (GClosure     *closure,
                                                    GValue       *return_value,
                                                    guint         n_param_values,
                                                    const GValue *param_values,
                                                    gpointer      invocation_hint,
                                                    gpointer      marshal_data);

/* VOID:STRING,POINTER (hildon-desktop-marshalers.list:2) */
extern void g_cclosure_user_marshal_VOID__STRING_POINTER (GClosure     *closure,
                                                          GValue       *return_value,
                                                          guint         n_param_values,
                                                          const GValue *param_values,
                                                          gpointer      invocation_hint,
                                                          gpointer      marshal_data);

/* VOID:POINTER,POINTER (hildon-desktop-marshalers.list:3) */
extern void g_cclosure_user_marshal_VOID__POINTER_POINTER (GClosure     *closure,
                                                           GValue       *return_value,
                                                           guint         n_param_values,
                                                           const GValue *param_values,
                                                           gpointer      invocation_hint,
                                                           gpointer      marshal_data);

/* VOID:POINTER,POINTER,POINTER (hildon-desktop-marshalers.list:4) */
extern void g_cclosure_user_marshal_VOID__POINTER_POINTER_POINTER (GClosure     *closure,
                                                                   GValue       *return_value,
                                                                   guint         n_param_values,
                                                                   const GValue *param_values,
                                                                   gpointer      invocation_hint,
                                                                   gpointer      marshal_data);

/* OBJECT:VOID (hildon-desktop-marshalers.list:5) */
extern void g_cclosure_user_marshal_OBJECT__VOID (GClosure     *closure,
                                                  GValue       *return_value,
                                                  guint         n_param_values,
                                                  const GValue *param_values,
                                                  gpointer      invocation_hint,
                                                  gpointer      marshal_data);

G_END_DECLS

#endif /* __g_cclosure_user_marshal_MARSHAL_H__ */

