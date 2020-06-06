import { Router } from 'express'
const router = Router();

router.get('/',(req, res) => res.send('./src/index.html'));


export default router;