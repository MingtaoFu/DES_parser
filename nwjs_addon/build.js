var NwBuilder = require('nw-builder');
var nw = new NwBuilder({
    files: './nwfile/**/**', // use the glob format
    platforms: ['win64', 'linux64'],
    version: '0.22.3'
});

//Log stuff you want

nw.on('log',  console.log);

// Build returns a promise
nw.build().then(function () {
   console.log('all done!');
}).catch(function (error) {
    console.error(error);
});
