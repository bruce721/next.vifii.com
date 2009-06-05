/* 
 * GPL license, Copyright (c) 2006 by Nokia Corporation                       
 *                                                                            
 * Authors:                                                                   
 *      Michael Dominic Kostrzewa <michael.kostrzewa@nokia.com>               
 *                                                                            
 * This program is free software; you can redistribute it and/or modify it    
 * under the terms of the GNU General Public License as published by the      
 * Free Software Foundation, version 2.                                                                   
 *                                                                            
 * This program is distributed in the hope that it will be useful, but        
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   
 * for more details.                                                          
 *                                                                            
 * You should have received a copy of the GNU General Public License along    
 * with this program; if not, write to the Free Software Foundation, Inc., 59 
 * Temple Place - Suite 330, Boston, MA 02111-1307, USA.                      
 *
 */

#include "outliner.h"

/* Do the work */
GdkPixbuf*                      process (Template *templ)
{
        GSList *iterator = NULL;
        int counter = 0;
        GdkPixbuf *composite_image = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8, templ->Width, templ->Height);
        
        g_return_val_if_fail (templ != NULL, NULL);

        if (composite_image == NULL) {
                g_printerr ("ERROR: Failed to allocate memory for new image (%d x %d)!\n", templ->Width, templ->Height);
                exit (128);
        }

        /* Reset the pixbuf to transparent white */
        gdk_pixbuf_fill (composite_image, 0xffffff00);

        /* Process all the images */
        for (iterator = templ->ElementList; iterator; iterator = g_slist_next (iterator)) {
                Element *element = (Element *) iterator->data;
                GdkPixbuf *image = NULL;
                GdkPixbuf *border_img = NULL;
                GdkPixbuf *h_border_img = NULL;
                GdkPixbuf *v_border_img = NULL;

                if (element->X > templ->Width || 
                    element->Y > templ->Height ||
                    element->X + element->Width > templ->Width ||
                    element->Y + element->Height > templ->Height) {
                        g_printerr ("WARNING: Element '%s' is out of bounds (%d %d %d %d)!\n", element->Name,  
                                   element->X, element->Y, element->Width, element->Height);
                } else { 
                        image = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8, element->Width, element->Height);

                        if (counter % 5 == 0) 
                                gdk_pixbuf_fill (image, 0xff660030);
                        else if (counter % 5 == 1)
                                gdk_pixbuf_fill (image, 0x9ade0030);
                        else if (counter % 5 == 2)
                                gdk_pixbuf_fill (image, 0x9eabb030);
                        else if (counter % 5 == 3)
                                gdk_pixbuf_fill (image, 0x0084c830);
                        else
                                gdk_pixbuf_fill (image, 0xc04ec230);
                                
                        /* Create a black outline */
                        /* TOP AND BOTTOM */
                        h_border_img = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8, element->Width, 1);
                        gdk_pixbuf_fill (h_border_img, 0x000000ff);
                        gdk_pixbuf_copy_area (h_border_img, 0, 0, element->Width, 1, 
                                              image, 0, 0);
                        gdk_pixbuf_copy_area (h_border_img, 0, 0, element->Width, 1, 
                                              image, 0, element->Height - 1);
                                              
                        /* LEFT AND RIGHT */
                        v_border_img = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8, 1, element->Height);
                        gdk_pixbuf_fill (v_border_img, 0x000000ff);
                        gdk_pixbuf_copy_area (v_border_img, 0, 0, 1, element->Height, 
                                              image, 0, 0);
                        gdk_pixbuf_copy_area (v_border_img, 0, 0, 1, element->Height, 
                                              image, element->Width - 1, 0); 
                                
                        if (element->BorderTop > 0) {
                                border_img = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8, element->Width, element->BorderTop);
                                gdk_pixbuf_fill (border_img, 0x000000ff);
                                gdk_pixbuf_composite (border_img, image, 0, 0, element->Width, element->BorderTop, 
                                      0, 0, 1.0, 1.0, GDK_INTERP_NEAREST, 32);
                                gdk_pixbuf_unref (border_img);
                                
                                gdk_pixbuf_copy_area (h_border_img, 0, 0, element->Width, 1, 
                                                      image, 0, element->BorderTop - 1);
                        }
                        
                        if (element->BorderBottom > 0) {
                                border_img = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8, element->Width, element->BorderBottom);
                                gdk_pixbuf_fill (border_img, 0x000000ff);
                                gdk_pixbuf_composite (border_img, image, 0, element->Height - element->BorderBottom, element->Width, element->BorderBottom, 
                                      0, 0, 1.0, 1.0, GDK_INTERP_NEAREST, 32);
                                gdk_pixbuf_unref (border_img);
                                
                                gdk_pixbuf_copy_area (h_border_img, 0, 0, element->Width, 1, 
                                                      image, 0, element->Height - element->BorderBottom);
                        }
                        
                       if (element->BorderLeft > 0) {
                                border_img = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8, element->BorderLeft, element->Height);
                                gdk_pixbuf_fill (border_img, 0x000000ff);
                                gdk_pixbuf_composite (border_img, image, 0, 0, element->BorderLeft, element->Height, 
                                      0, 0, 1.0, 1.0, GDK_INTERP_NEAREST, 32);
                                gdk_pixbuf_unref (border_img);
                                
                                gdk_pixbuf_copy_area (v_border_img, 0, 0, 1, element->Height, 
                                                      image, element->BorderLeft - 1, 0);      
                        }
                        
                        if (element->BorderRight > 0) {
                                border_img = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8, element->BorderRight, element->Height);
                                gdk_pixbuf_fill (border_img, 0x000000ff);
                                gdk_pixbuf_composite (border_img, image, element->Width - element->BorderRight, 0, element->BorderRight, element->Height, 
                                      0, 0, 1.0, 1.0, GDK_INTERP_NEAREST, 32);
                                gdk_pixbuf_unref (border_img); 
                                
                                gdk_pixbuf_copy_area (v_border_img, 0, 0, 1, element->Height, 
                                                      image, element->Width - element->BorderRight, 0);     
                        }
                        
                        gdk_pixbuf_unref (h_border_img);
                        gdk_pixbuf_unref (v_border_img);
                }

                if (image != NULL) {
                        /* Draw the stuff over the composite image */
                        gdk_pixbuf_copy_area (image, 0, 0, element->Width, element->Height, 
                                              composite_image, element->X, element->Y);
                        //g_print ("Outlined %s\n", element->Name);
                        
                        gdk_pixbuf_unref (image);
                }

                counter++;
        }

        /* Process all the colors */
        for (iterator = templ->ColorList; iterator; iterator = g_slist_next (iterator)) {
        
                Color *color = (Color *) iterator->data;
                GdkPixbuf *image = NULL;

                if (color->X - 4 > templ->Width || 
                    color->Y - 4 > templ->Height ||
                    color->X + 4 > templ->Width ||
                    color->Y + 4 > templ->Height) {
                        g_printerr ("WARNING: Color '%s' is out of bounds (%d %d)!\n", color->Name,  
                                    color->X, color->Y);
                } else { 
                        image = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8, 8, 8);

                        if (counter % 5 == 0) 
                                gdk_pixbuf_fill (image, 0xffff00ff);
                        else if (counter % 5 == 1)
                                gdk_pixbuf_fill (image, 0xffee00ff);
                        else if (counter % 5 == 2)
                                gdk_pixbuf_fill (image, 0xffdd00ff);
                        else if (counter % 5 == 3)
                                gdk_pixbuf_fill (image, 0xffcc00ff);
                        else
                                gdk_pixbuf_fill (image, 0xffbb00ff);
                }

                if (image != NULL) {
                        /* Draw the stuff over the composite image */
                        gdk_pixbuf_copy_area (image, 0, 0, 8, 8, 
                                              composite_image, color->X - 4, color->Y - 4);
                        //g_print ("Outlined color %s\n", color->Name);
                        
                        gdk_pixbuf_unref (image);
                }

                counter++;
        }

        return composite_image;
}

void                            parse_rc (gchar *filename, Template *template, GScanner *scanner)
{
        GSList *iterator = NULL;
        GTokenType ttype = 0;
        gchar imgname[256];
        gint left, right, top, bottom;

        int gtkrc_fd = open (filename, O_RDONLY);
        
        if (gtkrc_fd < 0) {
                g_print ("Failed to open '%s'. Skipping ...\n", filename);
        } else {
                g_scanner_input_file (scanner, gtkrc_fd);
                scanner->input_name = filename;
                
                for (ttype = g_scanner_get_next_token (scanner); ttype != G_TOKEN_EOF; ttype = g_scanner_get_next_token (scanner)) {
                
                        if (ttype == G_TOKEN_IDENTIFIER && !strcmp(scanner->value.v_string, "image")) {
                        
                                while (ttype != G_TOKEN_RIGHT_CURLY) {
                                        ttype = g_scanner_get_next_token (scanner);
                                        
                                        if (ttype == G_TOKEN_IDENTIFIER && !strcmp(scanner->value.v_string, "file")) {
                                                ttype = g_scanner_get_next_token (scanner);
                                                
                                                if (ttype != G_TOKEN_EQUAL_SIGN)
                                                        break;
                                                        
                                                ttype = g_scanner_get_next_token (scanner);
                                                strcpy (imgname, g_path_get_basename (scanner->value.v_string));
                                        } else if (ttype == G_TOKEN_IDENTIFIER && !strcmp(scanner->value.v_string, "border")) {
                                                
                                                ttype = g_scanner_get_next_token (scanner);
                                                
                                                // scan all four values, skipping the commas
                                                ttype = g_scanner_get_next_token (scanner);
                                                ttype = g_scanner_get_next_token (scanner);
                                                if (ttype == G_TOKEN_INT)
                                                        left = scanner->value.v_int;
                                                else
                                                        break;
                                                ttype = g_scanner_get_next_token (scanner);
                                                ttype = g_scanner_get_next_token (scanner);
                                                if (ttype == G_TOKEN_INT)
                                                        right = scanner->value.v_int;
                                                else
                                                        break;
                                                ttype = g_scanner_get_next_token (scanner);
                                                ttype = g_scanner_get_next_token (scanner);
                                                if (ttype == G_TOKEN_INT)
                                                        top = scanner->value.v_int;
                                                else
                                                        break;
                                                ttype = g_scanner_get_next_token (scanner);
                                                ttype = g_scanner_get_next_token (scanner);
                                                if (ttype == G_TOKEN_INT)
                                                        bottom = scanner->value.v_int;
                                                else
                                                        break;
                                                        
                                                // got valid border values, so look for a corresponding entry in the elements list
                                                for (iterator = template->ElementList; iterator; iterator = g_slist_next (iterator)) {
                                                        Element *element = (Element *) iterator->data;
                                                       
                                                        if (strcmp (element->Name, imgname))
                                                                continue;
                                                                
                                                        element->BorderLeft = left;
                                                        element->BorderRight = right;
                                                        element->BorderTop = top;
                                                        element->BorderBottom = bottom;
                                                        
                                                        break;     
                                                }
                                                        
                                                // skip the right curly
                                                ttype = g_scanner_get_next_token (scanner);
                                                ttype = g_scanner_get_next_token (scanner);   
                                        }                                                                                        
                                }
                        }
                }
        }
        
        close (gtkrc_fd);
}

/* Shows the initial copyright/info banner */
void                            show_banner (void)
{
        g_print ("Outliner tool by Michael Dominic K. and Tuomas Kuosmanen\n");
        g_print ("Copyright 2006 by Nokia Corporation.\n\n");
}

/* Show some info about basic usage of the tool */
void                            show_usage (void)
{
        g_print ("Usage: %s <layout> <outputimage> [rcdirectory]\n\n", g_get_prgname ());
        g_print ("This tool will create an outline image that shows the slicing guides. \n"
                 "If rcdirectory is given, it will look for .rc files in this directory\n"
                 "and gather matching border values from there.\n"
                 "You can use this image in your graphics program to check if your drawings\n"
                 "fit the proper areas. \n\n");
}

/* This is the place where we came in... */
int                             main (int argc, char **argv)
{
        gchar *template_file = NULL;
        gchar *output_image_file = NULL;
        gchar *rc_directory = NULL;
        int return_val = 0;
        Template *template = NULL;
        GdkPixbuf *output_image = NULL;
        GScanner *scanner = NULL;


	g_type_init ();
        g_set_prgname (g_basename (argv [0]));

        /* Check the args... */
        if (argc < 3) {
                show_banner ();
                show_usage ();
                g_printerr ("Not enough arguments given!\n");
                goto Error;
        }

        /* Get file vals */
        template_file = argv [1];
        output_image_file = argv [2];

        if (template_file == NULL || output_image_file == NULL) {
                show_banner ();
                show_usage ();
                g_printerr ("Bad arguments given!\n");
                goto Error;
        }
        
        if (argc > 3) {
                rc_directory = argv [3];
        }

        /* Check the template file... */
        if (! g_file_test (template_file, G_FILE_TEST_EXISTS)) {
                g_printerr ("ERROR: %s not found!\n", template_file);
                goto Error;
        }

        /* Read the template file. That spits out errors too */
        template = read_template (template_file);

        if (template == NULL)
                goto Error;
                
                
        /* Get border values from a directory containing .rc files */
        if (rc_directory) {
                if (!g_file_test (rc_directory, G_FILE_TEST_IS_DIR)) {
                        g_printerr ("ERROR: %s is not a directory\n", rc_directory);
                        goto Error;
                }
                
                g_print ("\n\n** Using directory '%s' for rc files.\n\n", rc_directory);
                
                gchar *rc_file = NULL;
                GDir *rc_dir = g_dir_open (rc_directory, 0, NULL);
                
                scanner = g_scanner_new (NULL);
                while (rc_file = g_dir_read_name (rc_dir)) {
                        parse_rc (g_build_filename (rc_directory, rc_file, NULL), template, scanner);
                }
                g_scanner_destroy (scanner);
                g_dir_close (rc_dir);
        } else {
                g_print ("\n\n** No rc directory specified, borders will not be generated.\n\n");
        }

        /* Basic info */
        show_template (template);

        output_image = process (template);
        g_print ("\n");

        /* Save the resulting image */
        if (output_image != NULL) {
                g_print ("Saving a '%s' image, this might take a while...\n", output_image_file);
                gdk_pixbuf_save (output_image, output_image_file, "png", NULL, NULL);
        }
        
        g_print ("Done!\n");

        goto Done;

Error:
        return_val = 128;
        
Done:
        if (template != NULL)
                free_template (template);

        if (output_image != NULL)
                gdk_pixbuf_unref (output_image);

        g_print ("\n");
        return return_val;
}
