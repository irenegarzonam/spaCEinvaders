import java.net.*;
import java.io.*;

public class Server {
    private ServerSocket serverSocket;
    private Socket clientSocket;
    private PrintWriter out;
    private BufferedReader in;

    public void start(int port) {
        try {
            // Create the server socket
            serverSocket = new ServerSocket(port);
            System.out.println("Server started on port " + port);

            // Wait for a client to connect
            System.out.println("Waiting for a client...");
            clientSocket = serverSocket.accept();
            System.out.println("Client connected: " + clientSocket);

            // Create the output and input streams
            out = new PrintWriter(clientSocket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
        } catch (IOException e) {
            System.out.println("Error starting server: " + e.getMessage());
        }
    }

    public void stop() {
        try {
            if (out != null) {
                out.close();
            }
            if (in != null) {
                in.close();
            }
            if (clientSocket != null) {
                clientSocket.close();
            }
            if (serverSocket != null) {
                serverSocket.close();
            }
            System.out.println("Server stopped");
        } catch (IOException e) {
            System.out.println("Error stopping server: " + e.getMessage());
        }
    }

    public void sendToClient(String message) {
        out.println(message);
    }

    public String receiveFromClient() throws IOException {
        String inputLine = in.readLine();
        System.out.println("Message received from client: " + inputLine);
        return inputLine;
    }

    public static void main(String[] args) {
        Server server = new Server();
        server.start(12345);
        Game game = new Game(server);
        game.updateGame();
        while (game.activeGame) {
            try {
                Thread.sleep(1);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        server.stop();
    }
}

