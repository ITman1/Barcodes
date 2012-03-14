///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.listviews
// File:       FileListAdapter.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the adapter which adapts the paths of the files to the
//             graphical output (list items).
///////////////////////////////////////////////////////////////////////////////

package com.android.listviews;
import java.io.File;
import java.util.ArrayList;

import android.app.Activity;
import android.graphics.drawable.Drawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.android.qrreader.R;
import com.filesystem.Filename;

/**
 * Class extends ArrayAdapter<String> and implements the adapter for the ListView.
 * Adapted are file paths to the views that contain the file icons and filename.
 * 
 * @version 1.0
 */
public class FileListAdapter extends ArrayAdapter<String> {
    
    /**
     * The Class which converts file extension to the proper icon.
     */
    private class ExtensionToIcon {
        
        /** The context. */
        private final Activity context;
        
        /** The file prefix. */
        public final String extension;
        
        /** The resource name of the icon. */
        public final String resourceName;
        
        /**
         * Instantiates a new extension-icon conversion object.
         *
         * @param context The context.
         * @param extension The file extension.
         * @param resourceName The resource name of the icon.
         */
        public ExtensionToIcon(Activity context, String extension, String resourceName) {
            this.context = context;
            this.extension = extension;
            this.resourceName = resourceName;
        }
        
        /**
         * Gets the icon assigned to this file extension.
         *
         * @return The icon assigned to this file extension.
         */
        public Drawable getDrawable() {
            String resourceURI = "drawable/" + resourceName;
            int imageResource =  context.getResources().getIdentifier(resourceURI, null, context.getPackageName());
            return context.getResources().getDrawable(imageResource);
        }
    }
    
    /** The context. */
    private final Activity context;
    
    /** The paths contained in the list view. */
    private ArrayList<String> paths;
    
    /** The array where will be stored objects converting the extension to icon. */
    private ExtensionToIcon extensionsToIcons[];
    
    /** Specifies whether has the list view goto parent item. If true, then the 
     * first item will be not taken as the regular file/directory. */
    private boolean hasGotoParentRow                = true;

    /**
     * The Class ViewHolder which groups the display elements on the view. 
     */
    static class ViewHolder {
        
        /** The text of the filename. */
        public TextView text;
        
        /** The image. */
        public ImageView image;
    }

    /**
     * Instantiates a new file list adapter to the views.
     *
     * @param context The context.
     * @param paths The list of the paths that should be displayed in the list view.
     * @see android.widget.ArrayAdapter ArrayAdapter
     */
    public FileListAdapter(Activity context, ArrayList<String> paths) {
        super(context, R.layout.filelist_row, paths);
        this.context = context;
        this.paths = paths;
        loadFileIcons();    // Load file icons from the resource
    }
    
    /**
     * Loads the file icons from the resource array and creates the conversion array {@link #extensionsToIcons}.
     */
    private void loadFileIcons() {
        // loading extension and icon array from the resources
        String[] filetype_suffixes = context.getResources().getStringArray(R.array.FileListAdapter_filetype_suffixes);
        String[] filetype_drawables = context.getResources().getStringArray(R.array.FileListAdapter_filetype_drawables);

        // creating conversion array extensionsToIcons
        extensionsToIcons = new ExtensionToIcon[filetype_suffixes.length];
        for (int i = 0; i < filetype_suffixes.length; i++) {
            extensionsToIcons[i] = new ExtensionToIcon(context, filetype_suffixes[i], filetype_drawables[i]);
        }
        
    }
       
    /** 
     * Adapts the file path to the view which consists of the icon and filename.
     * 
     * @see android.widget.ArrayAdapter#getView(int, android.view.View, android.view.ViewGroup)
     */
    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        View rowView = convertView;
        
        // An old view is not created yet, so inflate it from the XML
        if (rowView == null) {
            // Inflate list row
            LayoutInflater inflater = context.getLayoutInflater();
            rowView = inflater.inflate(R.layout.filelist_row, null);
            // To prevent calling findViewById in future we store elements into  
            // the viewHolder which will be inserted into view as the tag.
            ViewHolder viewHolder = new ViewHolder();
            viewHolder.text = (TextView) rowView.findViewById(R.id.file_name);
            viewHolder.image = (ImageView) rowView.findViewById(R.id.file_icon);
            rowView.setTag(viewHolder);
        }

        ViewHolder holder = (ViewHolder) rowView.getTag();  // Getting elements of this view
        
        if (hasGotoParentRow && position == 0) {    // Adapt this view as the goto parent item
            holder.text.setText(R.string.FileListAdapter_goto_parent);
            holder.image.setImageResource(R.drawable.file_goto_parent);
        } else {                                    // Adapt this view as the file/folder
            String path = paths.get(position);
            Drawable icon = null;
            
            // Exploring extension-icon array and trying to get the icon
            for (ExtensionToIcon extToIcon : extensionsToIcons) {
                if (extToIcon.extension.equalsIgnoreCase(Filename.getExtension(path, '.'))) {
                    icon = extToIcon.getDrawable();
                    break;
                }
            }
            
            // Setting the item text as the filename
            holder.text.setText(Filename.getFullFilename(path, File.separatorChar));
            
            // Setting the icon
            File file = new File(path);
            if (file.isDirectory()) {   // File is directory
                holder.image.setImageResource(R.drawable.file_folder);
            } else {
                if (icon != null) {     // File has associated the icon
                    holder.image.setImageDrawable(icon);
                } else {                // File has not associated the icon
                    holder.image.setImageResource(R.drawable.file_regular);
                }
            }
        }

        return rowView; // Return the adapted view
    }
    
    /**
     * Notifies this adapter that list view will have the goto parent item.
     * Should be called before {@link android.app.ListActivity#setListAdapter(android.widget.ListAdapter) 
     * setListAdapter(ListAdapter)}
     *
     * @param has True when the list view will have goto parent item, otherwise false.
     */
    public void hasGotoParentRow(boolean has) {
        hasGotoParentRow = has;
    }
    
}

//End of the file