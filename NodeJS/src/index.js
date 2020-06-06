import '@babel/polyfill';

import app from "./server.js"

async function main() {
	app.listen(app.get('port'));
	console.log('Server on port ', app.get('port'));
}

main();