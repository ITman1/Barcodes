///////////////////////////////////////////////////////////////////////////////
// Project:   QR Reader for Android
// Package:   com.adapter
// File:      AdapterRegistry.java
// Date:      March 2012
// Author:    Radim Loskot
// E-mail:    xlosko01(at)stud.fit.vutbr.cz
//
// Brief:     Contains the Adapter registry for registering the 
//            adapters and later searching by adaptee/result classes.
///////////////////////////////////////////////////////////////////////////////

package com.adapter;

import java.util.HashMap;
import java.util.Map;

import android.util.Pair;

/**
 * The global adapter registry for registering the adapters ({@link Adapter}) 
 * which converts objects to the diametrically different objects.
 * 
 * @version 1.0
 * @see Adapter
 */
final public class AdapterRegistry {
    
    /** The global instance of the adapter registry. */
    private static AdapterRegistry registryInstance;
    
    /** The map of the adapter classes. Adapters are being mapped 
     *  by the adaptee and result classes. */
    private Map<Pair<Class<?>, Class<?> >, Class<?> > adapterMap = 
            new HashMap<Pair<Class<?>, Class<?> >, Class<?> >();
    
    /**
     * Instantiates a new adapter registry.
     */
    private AdapterRegistry() {}

    /**
     * Registers a new adapter to the registry. When the adapter already exists
     * in the registry it is not overwritten.
     *
     * @param adapter The class of the adapter which is being registered. 
     * The class must implement {@link Adapter} interface.
     * @return True when the adapter is successfully registered otherwise
     * returns false.
     */
    public boolean registerAdapter(Class<?> adapter) {
        try {
            Object objAdapter = adapter.newInstance();
            
            if (objAdapter instanceof Adapter) {
                Adapter adapterInstance = (Adapter)objAdapter;
                
                // Create the search key
                Pair<Class<?>, Class<?> > fromToClass = new Pair<Class<?>, Class<?> >(
                        adapterInstance.getAdapteeClass(), adapterInstance.getResultClass()
                );
                
                // Insert only when the search key is not already present in the registry
                if (!adapterMap.containsKey(fromToClass)) {
                    adapterMap.put(fromToClass, adapter);
                    return true;
                }
            }
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (ClassCastException e) {
            e.printStackTrace();
        }
        return false;
    }
    
    /**
     * Gets the instance of the adapter registry.
     *
     * @return The instance of the adapter registry.
     */
    public static AdapterRegistry getRegistry() {
        return registryInstance = (registryInstance == null)? new AdapterRegistry() : registryInstance;
    }
    
    /**
     * Gets the demanded adapter specifying the adaptee and result class.
     *
     * @param adapteeClass The adaptee class, which should be adapted to result class.
     * @param resultClass The result class, which should the adapter adapt.
     * @return The adapter which should implement the adapt method.
     */
    public Adapter getAdapter(Class<?> adapteeClass, Class<?> resultClass) {
        try {
            Class<?> adapterClass = adapterMap.get(new Pair<Class<?>, Class<?> >(adapteeClass, resultClass));
            return (adapterClass != null)? (Adapter)adapterClass.newInstance() : null;
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (ClassCastException e) {
            e.printStackTrace();
        }
        return null;
    }
}

// End of the file
