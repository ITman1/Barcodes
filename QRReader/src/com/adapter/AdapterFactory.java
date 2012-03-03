package com.adapter;

import java.util.HashMap;
import java.util.Map;

import android.util.Pair;

final public class AdapterFactory {
    private static AdapterFactory factoryInstance;
    private Map<Pair<Class<?>, Class<?> >, Class<?> > adapterMap = new HashMap<Pair<Class<?>, Class<?> >, Class<?> >();
    
    private AdapterFactory() {}

    public boolean registerAdapter(Class<?> adapter) {
        try {
            Object objAdapter = adapter.newInstance();
            if (objAdapter instanceof Adapter) {
                Adapter adapterInstance = (Adapter)objAdapter;
                Pair<Class<?>, Class<?> > fromToClass = new Pair<Class<?>, Class<?> >(
                        adapterInstance.getAdapteeClass(), adapterInstance.getResultClass()
                );
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
    
    public static AdapterFactory getFactory() {
        return factoryInstance = (factoryInstance == null)? new AdapterFactory() : factoryInstance;
    }
    
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
