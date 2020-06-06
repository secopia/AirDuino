import express, { json } from 'express'

const app = express()


//routes

import IndexRoutes from './routes/index.routes.js'
import TasksRoutes from './routes/tasks.routes.js'
import InsertsRoutes from './routes/inserts.routes.js'


//Settings
app.set('port', process.env.PORT || 3000);

//Middlewares
app.use(json());

//Routes
app.use(IndexRoutes);
app.use('/tasks', TasksRoutes);
app.use('/inserts', InsertsRoutes);

export default app;