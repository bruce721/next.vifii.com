
#ifndef ___hildon_marshal_MARSHAL_H__
#define ___hildon_marshal_MARSHAL_H__

#include	<glib-object.h>

G_BEGIN_DECLS

/* BOOLEAN:ENUM (hildon-marshalers.list:26) */
extern void _hildon_marshal_BOOLEAN__ENUM (GClosure     *closure,
                                           GValue       *return_value,
                                           guint         n_param_values,
                                           const GValue *param_values,
                                           gpointer      invocation_hint,
                                           gpointer      marshal_data);

/* BOOLEAN:INT,INT,INT (hildon-marshalers.list:27) */
extern void _hildon_marshal_BOOLEAN__INT_INT_INT (GClosure     *closure,
                                                  GValue       *return_value,
                                                  guint         n_param_values,
                                                  const GValue *param_values,
                                                  gpointer      invocation_hint,
                                                  gpointer      marshal_data);

/* BOOLEAN:POINTER (hildon-marshalers.list:28) */
extern void _hildon_marshal_BOOLEAN__POINTER (GClosure     *closure,
                                              GValue       *return_value,
                                              guint         n_param_values,
                                              const GValue *param_values,
                                              gpointer      invocation_hint,
                                              gpointer      marshal_data);

/* VOID:OBJECT (hildon-marshalers.list:29) */
#define _hildon_marshal_VOID__OBJECT	g_cclosure_marshal_VOID__OBJECT

/* VOID:VOID (hildon-marshalers.list:30) */
#define _hildon_marshal_VOID__VOID	g_cclosure_marshal_VOID__VOID

G_END_DECLS

#endif /* ___hildon_marshal_MARSHAL_H__ */

