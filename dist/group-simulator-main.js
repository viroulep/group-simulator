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

// FIXME: Ideally, this would be created from model_props.def/setup_props.def
// Maybe look into CMake configuration?
export const SettingsDescription = {
  setup: {
    judges: {
      text: 'Judges',
      desc: 'Number of judges',
    },
    scramblers: {
      text: 'Scramblers',
      desc: 'Number of scramblers',
    },
    runners: {
      text: 'Runners',
      desc: 'Number of runners (if the model uses them)',
    },
    cutoff: {
      text: 'Cutoff',
      desc: 'Cutoff time, in seconds',
    },
    time_limit: {
      text: 'Time limit',
      desc: 'Time limit time, in seconds (or cumulative time limit, for blindfolded events)',
    },
    cubes_per_runner: {
      text: 'Cubes per runner',
      desc: 'The number of cubes a runner can run in',
    },
    extra_rate: {
      text: 'Extra rate',
      desc: 'Number of extra happening per 100 attempts',
    },
    miscramble_rate: {
      text: 'Miscramble rate',
      desc: 'Number of miscrambles happening per 100 scrambles',
    },
  },
  model: {
    init_group: {
      text: 'Group start',
      desc: 'Flat delay before the first cubes becomes ready',
    },
    run_in: {
      text: 'Run in',
      desc: 'Time taken by the judge/runner from the scrambling table to the station, picking up the competitor',
    },
    competitor_ready: {
      text: 'Attempt setup',
      desc: 'Time taken for the competitor to become ready and inspect',
    },
    competitor_cleanup: {
      text: 'Attempt cleanup',
      desc: 'Time taken once the solve is over',
    },
    extra_delay: {
      text: 'Extra delay',
      desc: 'Delay to resolve an extra',
    },
    run_out: {
      text: 'Run out',
      desc: 'Time taken by the judge/runner from the station to the scrambling table',
    },
    shutdown_group: {
      text: 'Group end',
      desc: 'Flat delay at the end of the group',
    },
  },
  scrambling: {
    333: {
      text: '3x3x3',
      desc: 'Time to scramble a 3x3',
    },
    222: {
      text: '2x2x2',
      desc: 'Time to scramble a 2x2',
    },
    444: {
      text: '4x4x4',
      desc: 'Time to scramble a 4x4',
    },
    555: {
      text: '5x5x5',
      desc: 'Time to scramble a 5x5',
    },
    666: {
      text: '6x6x6',
      desc: 'Time to scramble a 6x6',
    },
    777: {
      text: '7x7x7',
      desc: 'Time to scramble a 7x7',
    },
    '333bf': {
      text: '3x3x3 Blindfolded',
      desc: 'Time to scramble a 3x3 for 3x3 Blindfolded',
    },
    '333oh': {
      text: '3x3x3 One-Handed',
      desc: 'Time to scramble a 3x3 for 3x3 One-Handed',
    },
    clock: {
      text: 'Clock',
      desc: 'Time to scramble a Clock',
    },
    minx: {
      text: 'Megaminx',
      desc: 'Time to scramble a Megaminx',
    },
    pyram: {
      text: 'Pyraminx',
      desc: 'Time to scramble a Pyraminx',
    },
    skewb: {
      text: 'Skewb',
      desc: 'Time to scramble a Skewb',
    },
    sq1: {
      text: 'Square-1',
      desc: 'Time to scramble a Square-1',
    },
    '444bf': {
      text: '4x4x4 Blindfolded',
      desc: 'Time to scramble a 4x4 for 4x4 Blindfolded',
    },
    '555bf': {
      text: '5x5x5 Blindfolded',
      desc: 'Time to scramble a 5x5 for 5x5 Blindfolded',
    },
  },
};
