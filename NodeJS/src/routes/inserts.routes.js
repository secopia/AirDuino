import {Router} from 'express'
const router = Router();

import { connect } from '../database'

router.get('/', async (req, res)=> {
	const db = await connect();
	var ID = req.query.ID;
	var CO2 = parseFloat(req.query.CO2);
	var NO = parseFloat(req.query.NO);
	var NH3 = parseFloat(req.query.NH3);
	var nDate = new Date().toLocaleString('pl', { timeZone: 'Europe/Warsaw'});
	var nDate = nDate.replace("/", "-");
	var nDate = nDate.replace(", ", " ");
	const task = {
		ID: req.query.ID,
		CO2: parseFloat(req.query.CO2),
		NO: parseFloat(req.query.NO),
		NH3: parseFloat(req.query.NH3),
		date : new Date(nDate)
	};
	const result = await db.collection('Pruebas').insertOne(task);
	res.send('Data inserted correctly!');
})


router.get('/g', async (req, res)=> {
	const db = await connect();
	var nDate = new Date().toLocaleString('pl', { timeZone: 'Europe/Warsaw'});
	var nDate = nDate.replace("/", "-");
	var nDate = nDate.replace(", ", " ");
	const task = {
		ID: "Greece",
		CO2: parseFloat(req.query.CO2),
		NO: parseFloat(req.query.NO),
		NH3: parseFloat(req.query.NH3),
		CO: parseFloat(req.query.CO),
		PM10: parseFloat(req.query.PM10),
		PM25: parseFloat(req.query.PM25),
		date : new Date(nDate)
	};
	const result = await db.collection('DatosDispositivosFijos').insertOne(task);
	res.send('Data inserted correctly!');
})

router.get('/b', async (req, res)=> {
	const db = await connect();
	var nDate = new Date().toLocaleString('pl', { timeZone: 'Europe/Warsaw'});
	var nDate = nDate.replace("/", "-");
	var nDate = nDate.replace(", ", " ");
	const task = {
		ID: "Bulgaria",
		CO2: parseFloat(req.query.CO2),
		NO: parseFloat(req.query.NO),
		NH3: parseFloat(req.query.NH3),
		CO: parseFloat(req.query.CO),
		PM10: parseFloat(req.query.PM10),
		PM25: parseFloat(req.query.PM25),
		date : new Date(nDate)
	};
	const result = await db.collection('DatosDispositivosFijos').insertOne(task);
	res.send('Data inserted correctly!');
})

router.get('/e', async (req, res)=> {
	const db = await connect();
	var nDate = new Date().toLocaleString('pl', { timeZone: 'Europe/Warsaw'});
	var nDate = nDate.replace("/", "-");
	var nDate = nDate.replace(", ", " ");
	const task = {
		ID: "Spain",
		CO2: parseFloat(req.query.CO2),
		NO: parseFloat(req.query.NO),
		NH3: parseFloat(req.query.NH3),
		CO: parseFloat(req.query.CO),
		PM10: parseFloat(req.query.PM10),
		PM25: parseFloat(req.query.PM25),
		date : new Date(nDate)
	};
	const result = await db.collection('DatosDispositivosFijos').insertOne(task);
	res.send('Data inserted correctly!');
})


router.get('/p', async (req, res)=> {
	const db = await connect();
	var nDate = new Date().toLocaleString('pl', { timeZone: 'Europe/Warsaw'});
	var nDate = nDate.replace("/", "-");
	var nDate = nDate.replace(", ", " ");
	const task = {
		ID: "Spain",
		CO2: parseFloat(req.query.CO2),
		NO: parseFloat(req.query.NO),
		NH3: parseFloat(req.query.NH3),
		Coordenadas: {
			Latitud: parseFloat(req.query.LA),
			Longitud: parseFloat(req.query.LO)
		},
		date : new Date(nDate)
	};
	const result = await db.collection('DatosDispositivosPortables').insertOne(task);
	res.send('Data inserted!');
})

export default router;