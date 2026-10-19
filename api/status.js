import { neon } from "@neondatabase/serverless";

const sql = neon(process.env.DATABASE_URL);

export default async function handler(req, res) {
  res.setHeader("Access-Control-Allow-Origin", "*");
  res.setHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  res.setHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
  if (req.method === "OPTIONS") return res.status(200).end();

  if (req.method === "POST") {
    if (req.headers.authorization !== `Bearer ${process.env.API_TOKEN}`) {
      return res.status(401).json({ error: "unauthorized" });
    }
    const body = typeof req.body === "string" ? JSON.parse(req.body) : req.body;
    if (!["open", "away", "close"].includes(body.state)) {
      return res.status(400).json({ error: "invalid state" });
    }
    const rows = await sql`
      UPDATE door_status
      SET state = ${body.state}, updated_at = now()
      WHERE id = 1
      RETURNING *`;
    return res.status(200).json(rows[0]);
  }

  if (req.method === "GET") {
    const rows = await sql`SELECT * FROM door_status WHERE id = 1`;
    return res.status(200).json(rows[0]);
  }

  return res.status(405).json({ error: "method not allowed" });
}