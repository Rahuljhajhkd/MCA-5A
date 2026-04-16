const express = require("express");
const mongoose = require("mongoose");
const cors = require("cors");

const app = express();
app.use(express.json());
app.use(cors());

// MongoDB connect
mongoose.connect("mongodb://127.0.0.1:27017/employeedb")
.then(() => console.log("MongoDB Connected"))
.catch(err => console.log(err));

// Schema
const employeeSchema = new mongoose.Schema({
    name: String,
    department: String,
    salary: Number
});

const Employee = mongoose.model("Employee", employeeSchema);

// CREATE
app.post("/employees", async (req, res) => {
    const emp = new Employee(req.body);
    const data = await emp.save();
    res.json(data);
});

// READ ALL
app.get("/employees", async (req, res) => {
    const data = await Employee.find();
    res.json(data);
});

// READ ONE
app.get("/employees/:id", async (req, res) => {
    const data = await Employee.findById(req.params.id);
    res.json(data);
});

// UPDATE
app.put("/employees/:id", async (req, res) => {
    const data = await Employee.findByIdAndUpdate(req.params.id, req.body, { new: true });
    res.json(data);
});

// DELETE
app.delete("/employees/:id", async (req, res) => {
    await Employee.findByIdAndDelete(req.params.id);
    res.json({ message: "Deleted" });
});

// Server
app.listen(5000, () => console.log("Server running on port 5000"));
