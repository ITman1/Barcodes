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

public class FileListAdapter extends ArrayAdapter<String> {
    
    private class ExtensionToIcon {
        private final Activity context;
        public final String prefix;
        public final String resourceName;
        
        public ExtensionToIcon(Activity context, String prefix, String resourceName) {
            this.context = context;
            this.prefix = prefix;
            this.resourceName = resourceName;
        }
        
        public Drawable getDrawable() {
            String resourceURI = "drawable/" + resourceName;
            int imageResource =  context.getResources().getIdentifier(resourceURI, null, context.getPackageName());
            return context.getResources().getDrawable(imageResource);
        }
    }
    
    private final Activity context;
    private ArrayList<String> paths;
    
    private ExtensionToIcon extensionsToIcons[];
    private boolean hasGotoParentRow                = true;

    static class ViewHolder {
        public TextView text;
        public ImageView image;
    }

    public FileListAdapter(Activity context, ArrayList<String> paths) {
        super(context, R.layout.filelist_row, paths);
        this.context = context;
        this.paths = paths;
        loadFileIcons();
    }
    
    private void loadFileIcons() {
        String[] filetype_suffixes = context.getResources().getStringArray(R.array.FileListAdapter_filetype_suffixes);
        String[] filetype_drawables = context.getResources().getStringArray(R.array.FileListAdapter_filetype_drawables);

        extensionsToIcons = new ExtensionToIcon[filetype_suffixes.length];
        for (int i = 0; i < filetype_suffixes.length; i++) {
            extensionsToIcons[i] = new ExtensionToIcon(context, filetype_suffixes[i], filetype_drawables[i]);
        }
        
    }
       
    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        View rowView = convertView;
        
        if (rowView == null) {
            LayoutInflater inflater = context.getLayoutInflater();
            rowView = inflater.inflate(R.layout.filelist_row, null);
            ViewHolder viewHolder = new ViewHolder();
            viewHolder.text = (TextView) rowView.findViewById(R.id.file_name);
            viewHolder.image = (ImageView) rowView.findViewById(R.id.file_icon);
            rowView.setTag(viewHolder);
        }

        ViewHolder holder = (ViewHolder) rowView.getTag();
        if (hasGotoParentRow && position == 0) {
            holder.text.setText(R.string.FileListAdapter_goto_parent);
            holder.image.setImageResource(R.drawable.file_goto_parent);
        } else {
            String path = paths.get(position);
            Drawable icon = null;
            
            for (ExtensionToIcon extToIcon : extensionsToIcons) {
                if (extToIcon.prefix.equalsIgnoreCase(Filename.getExtension(path, '.'))) {
                    icon = extToIcon.getDrawable();
                    break;
                }
            }
            
            holder.text.setText(Filename.getFullFilename(path, File.separatorChar));
            File file = new File(path);
            
            if (file.isDirectory()) {
                holder.image.setImageResource(R.drawable.file_folder);
            } else {
                if (icon != null) { 
                    holder.image.setImageDrawable(icon);
                } else {
                    holder.image.setImageResource(R.drawable.file_regular);
                }
            }
        }

        return rowView;
    }
    
    public void hasGotoParentRow(boolean has) {
        hasGotoParentRow = has;
    }
    
}
