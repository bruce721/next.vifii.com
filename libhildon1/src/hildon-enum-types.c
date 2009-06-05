
/* Generated data (by glib-mkenums) */

#include "hildon.h"
#include <glib-object.h>

/* enumerations from "hildon-caption.h" */
GType
hildon_caption_status_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { HILDON_CAPTION_OPTIONAL, "HILDON_CAPTION_OPTIONAL", "optional" },
      { HILDON_CAPTION_MANDATORY, "HILDON_CAPTION_MANDATORY", "mandatory" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("HildonCaptionStatus", values);
  }
  return etype;
}
GType
hildon_caption_icon_position_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { HILDON_CAPTION_POSITION_LEFT, "HILDON_CAPTION_POSITION_LEFT", "left" },
      { HILDON_CAPTION_POSITION_RIGHT, "HILDON_CAPTION_POSITION_RIGHT", "right" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("HildonCaptionIconPosition", values);
  }
  return etype;
}

/* enumerations from "hildon-note.h" */
GType
hildon_note_type_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { HILDON_NOTE_TYPE_CONFIRMATION, "HILDON_NOTE_TYPE_CONFIRMATION", "confirmation" },
      { HILDON_NOTE_TYPE_CONFIRMATION_BUTTON, "HILDON_NOTE_TYPE_CONFIRMATION_BUTTON", "confirmation-button" },
      { HILDON_NOTE_TYPE_INFORMATION, "HILDON_NOTE_TYPE_INFORMATION", "information" },
      { HILDON_NOTE_TYPE_INFORMATION_THEME, "HILDON_NOTE_TYPE_INFORMATION_THEME", "information-theme" },
      { HILDON_NOTE_TYPE_PROGRESSBAR, "HILDON_NOTE_TYPE_PROGRESSBAR", "progressbar" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("HildonNoteType", values);
  }
  return etype;
}

/* enumerations from "hildon-number-editor.h" */
GType
hildon_number_editor_error_type_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { HILDON_NUMBER_EDITOR_ERROR_MAXIMUM_VALUE_EXCEED, "HILDON_NUMBER_EDITOR_ERROR_MAXIMUM_VALUE_EXCEED", "maximum-value-exceed" },
      { HILDON_NUMBER_EDITOR_ERROR_MINIMUM_VALUE_EXCEED, "HILDON_NUMBER_EDITOR_ERROR_MINIMUM_VALUE_EXCEED", "minimum-value-exceed" },
      { HILDON_NUMBER_EDITOR_ERROR_ERRONEOUS_VALUE, "HILDON_NUMBER_EDITOR_ERROR_ERRONEOUS_VALUE", "erroneous-value" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("HildonNumberEditorErrorType", values);
  }
  return etype;
}

/* enumerations from "hildon-time-editor.h" */
GType
hildon_date_time_error_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { HILDON_DATE_TIME_ERROR_NO_ERROR, "HILDON_DATE_TIME_ERROR_NO_ERROR", "no-error" },
      { HILDON_DATE_TIME_ERROR_MAX_HOURS, "HILDON_DATE_TIME_ERROR_MAX_HOURS", "max-hours" },
      { HILDON_DATE_TIME_ERROR_MAX_MINS, "HILDON_DATE_TIME_ERROR_MAX_MINS", "max-mins" },
      { HILDON_DATE_TIME_ERROR_MAX_SECS, "HILDON_DATE_TIME_ERROR_MAX_SECS", "max-secs" },
      { HILDON_DATE_TIME_ERROR_MAX_DAY, "HILDON_DATE_TIME_ERROR_MAX_DAY", "max-day" },
      { HILDON_DATE_TIME_ERROR_MAX_MONTH, "HILDON_DATE_TIME_ERROR_MAX_MONTH", "max-month" },
      { HILDON_DATE_TIME_ERROR_MAX_YEAR, "HILDON_DATE_TIME_ERROR_MAX_YEAR", "max-year" },
      { HILDON_DATE_TIME_ERROR_MIN_HOURS, "HILDON_DATE_TIME_ERROR_MIN_HOURS", "min-hours" },
      { HILDON_DATE_TIME_ERROR_MIN_MINS, "HILDON_DATE_TIME_ERROR_MIN_MINS", "min-mins" },
      { HILDON_DATE_TIME_ERROR_MIN_SECS, "HILDON_DATE_TIME_ERROR_MIN_SECS", "min-secs" },
      { HILDON_DATE_TIME_ERROR_MIN_DAY, "HILDON_DATE_TIME_ERROR_MIN_DAY", "min-day" },
      { HILDON_DATE_TIME_ERROR_MIN_MONTH, "HILDON_DATE_TIME_ERROR_MIN_MONTH", "min-month" },
      { HILDON_DATE_TIME_ERROR_MIN_YEAR, "HILDON_DATE_TIME_ERROR_MIN_YEAR", "min-year" },
      { HILDON_DATE_TIME_ERROR_EMPTY_HOURS, "HILDON_DATE_TIME_ERROR_EMPTY_HOURS", "empty-hours" },
      { HILDON_DATE_TIME_ERROR_EMPTY_MINS, "HILDON_DATE_TIME_ERROR_EMPTY_MINS", "empty-mins" },
      { HILDON_DATE_TIME_ERROR_EMPTY_SECS, "HILDON_DATE_TIME_ERROR_EMPTY_SECS", "empty-secs" },
      { HILDON_DATE_TIME_ERROR_EMPTY_DAY, "HILDON_DATE_TIME_ERROR_EMPTY_DAY", "empty-day" },
      { HILDON_DATE_TIME_ERROR_EMPTY_MONTH, "HILDON_DATE_TIME_ERROR_EMPTY_MONTH", "empty-month" },
      { HILDON_DATE_TIME_ERROR_EMPTY_YEAR, "HILDON_DATE_TIME_ERROR_EMPTY_YEAR", "empty-year" },
      { HILDON_DATE_TIME_ERROR_MIN_DURATION, "HILDON_DATE_TIME_ERROR_MIN_DURATION", "min-duration" },
      { HILDON_DATE_TIME_ERROR_MAX_DURATION, "HILDON_DATE_TIME_ERROR_MAX_DURATION", "max-duration" },
      { HILDON_DATE_TIME_ERROR_INVALID_CHAR, "HILDON_DATE_TIME_ERROR_INVALID_CHAR", "invalid-char" },
      { HILDON_DATE_TIME_ERROR_INVALID_DATE, "HILDON_DATE_TIME_ERROR_INVALID_DATE", "invalid-date" },
      { HILDON_DATE_TIME_ERROR_INVALID_TIME, "HILDON_DATE_TIME_ERROR_INVALID_TIME", "invalid-time" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("HildonDateTimeError", values);
  }
  return etype;
}

/* enumerations from "hildon-window.h" */
GType
hildon_window_clipboard_operation_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { HILDON_WINDOW_CO_COPY, "HILDON_WINDOW_CO_COPY", "copy" },
      { HILDON_WINDOW_CO_CUT, "HILDON_WINDOW_CO_CUT", "cut" },
      { HILDON_WINDOW_CO_PASTE, "HILDON_WINDOW_CO_PASTE", "paste" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("HildonWindowClipboardOperation", values);
  }
  return etype;
}

/* enumerations from "hildon-wizard-dialog.h" */
GType
hildon_wizard_dialog_response_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { HILDON_WIZARD_DIALOG_CANCEL, "HILDON_WIZARD_DIALOG_CANCEL", "cancel" },
      { HILDON_WIZARD_DIALOG_PREVIOUS, "HILDON_WIZARD_DIALOG_PREVIOUS", "previous" },
      { HILDON_WIZARD_DIALOG_NEXT, "HILDON_WIZARD_DIALOG_NEXT", "next" },
      { HILDON_WIZARD_DIALOG_FINISH, "HILDON_WIZARD_DIALOG_FINISH", "finish" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("HildonWizardDialogResponse", values);
  }
  return etype;
}

/* enumerations from "hildon-calendar.h" */
GType
hildon_calendar_display_options_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GFlagsValue values[] = {
      { HILDON_CALENDAR_SHOW_HEADING, "HILDON_CALENDAR_SHOW_HEADING", "show-heading" },
      { HILDON_CALENDAR_SHOW_DAY_NAMES, "HILDON_CALENDAR_SHOW_DAY_NAMES", "show-day-names" },
      { HILDON_CALENDAR_NO_MONTH_CHANGE, "HILDON_CALENDAR_NO_MONTH_CHANGE", "no-month-change" },
      { HILDON_CALENDAR_SHOW_WEEK_NUMBERS, "HILDON_CALENDAR_SHOW_WEEK_NUMBERS", "show-week-numbers" },
      { HILDON_CALENDAR_WEEK_START_MONDAY, "HILDON_CALENDAR_WEEK_START_MONDAY", "week-start-monday" },
      { 0, NULL, NULL }
    };
    etype = g_flags_register_static ("HildonCalendarDisplayOptions", values);
  }
  return etype;
}

/* Generated data ends here */

