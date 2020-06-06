import {Router} from 'express'
const router = Router();

import { connect } from '../database'


router.get('/P', async (req, res) => {
	const db = await connect();
	const result = await db.collection('DatosDispositivosPortables').find({}).sort({date:-1}).limit(20).toArray();
	console.log(result);
	res.json(result.reverse());
});

router.get('/F', async (req, res) => {
	const db = await connect();
	const result = await db.collection('DatosDispositivosFijos').find({}).sort({date:-1}).limit(20).toArray();
	res.json(result.reverse());
});

export default router;