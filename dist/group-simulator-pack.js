// This is a little helper to load the module in a webpack context
import simu from './group-simulator';

export const getSimu = (onLoaded) => {
  return simu({
    onRuntimeInitialized: onLoaded,
    locateFile: () => require('./group-simulator.wasm'),
  });
};

export const asObject = (nativeMap) => {
  // Turns a 'Map*' into a js object.
  const obj = {};
  const keys = nativeMap.keys();
  for (let i = 0; i < keys.size(); i++) {
    const k = keys.get(i);
    obj[k] = nativeMap.get(k);
  }
  return obj;
};

// Assumes MapClass is actually a class representing a vector...
// Eg: our MapStringInt class.
export const asMap = (MapClass, obj) => {
  const native = new MapClass();
  Object.entries(obj).map(([k, v]) => native.set(k, v));
  return native;
};

// Assumes VectorClass is actually a class representing a vector...
// Eg: our VectorTime or VectorString
export const asVector = (VectorClass, array) => {
  const ret = new VectorClass();
  array.forEach((el) => ret.push_back(el));
  return ret;
};

export const asArray = (vector) => {
  const ret = [];
  for (let i = 0; i < vector.size(); i++) {
    ret.push(vector.get(i));
  }
  return ret;
};
