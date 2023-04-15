import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class SpaceInvadersServer {
    import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

    public static class SpaceInvadersServer {
        private int port;
        private ServerSocket serverSocket;

        public SpaceInvadersServer(int port) {
            this.port = port;
        }

        public void start() {
            try {
                serverSocket = new ServerSocket(port);
                System.out.println("Server started on port " + port);

                while (true) {
                    Socket clientSocket = serverSocket.accept();
                    System.out.println("New client connected: " + clientSocket.getInetAddress().getHostName());

                    // Handle client connection using a new thread
                    SpaceInvadersClientHandler clientHandler = new SpaceInvadersClientHandler(clientSocket);
                    Thread clientThread = new Thread(clientHandler);
                    clientThread.start();
                }
            } catch (IOException e) {
                System.err.println("Error starting server on port " + port + ": " + e.getMessage());
            }
        }

        public void stop() {
            try {
                if (serverSocket != null) {
                    serverSocket.close();
                }
            } catch (IOException e) {
                System.err.println("Error stopping server: " + e.getMessage());
            }
        }

        public static void main(String[] args) {
            SpaceInvadersServer server = new SpaceInvadersServer(1234);
            server.start();
        }
    }

}
