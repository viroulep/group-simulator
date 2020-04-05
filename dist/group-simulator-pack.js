// This is a little helper to load the module in a webpack context
import simu from './group-simulator';

const getWasm = (onLoaded) => {
  return simu({
    onRuntimeInitialized: onLoaded,
    locateFile: () => require('./group-simulator.wasm'),
  });
};

export default getWasm;
