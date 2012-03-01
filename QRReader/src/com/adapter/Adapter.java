package com.adapter;

public interface Adapter {
    public Object getProvider(Object o);
    public Class<?> getAdapteeClass();
    public Class<?> getResultClass();
}
