
import { MongoClient, ServerApiVersion } from 'mongodb';
import dotenv from 'dotenv';
dotenv.config();

const uri = process.env.MONGODB_URI;

// Create a MongoClient with a MongoClientOptions object to set the Stable API version
// @ts-ignore
const client = new MongoClient(uri, {
  serverApi: {
    version: ServerApiVersion.v1,
    strict: true,
    deprecationErrors: true,
  }
});

async function isConnected() {
    try {
      await client.db().admin().command({ ping: 1 }); // ✅ Proper way to check if MongoDB is connected
      return true;
    } catch (error) {
      return false;
    }
  }

async function connectDB() {
  try {
    if (!await isConnected()) {
        // Connect the client to the server	(optional starting in v4.7)
        await client.connect();
    }

    // Send a ping to confirm a successful connection
    await client.db("admin").command({ ping: 1 });
    console.log("Pinged your deployment. You successfully connected to MongoDB!");
    return client;
  } catch(error) {
    console.error("Failed to connect to MongoDB:", error);
    throw error;
  }
}

// Function to close the connection (for cleanup)
async function closeDB() {
    await client.close();
    console.log("❌ MongoDB connection closed.");
  }

  async function getConnectionStats() {
    try {
      const stats = await client.db().admin().command({ serverStatus: 1 });
      console.log(`🔍 Active connections: ${stats.connections.current}`);
      return stats.connections.current; // Returns the number of current connections
    } catch (error) {
      console.error("❌ Error fetching connection stats:", error);
      return null;
    }
  }
//getConnectionStats();
//connectDB().catch(console.dir);
//closeDB();
export { client, connectDB, closeDB };
