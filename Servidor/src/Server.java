import java.net.*;
import java.io.*;

public class Server {
    private ServerSocket serverSocket;
    private Socket clientSocket;
    private PrintWriter out;
    private BufferedReader in;
    private boolean inicio1 = false;
    private boolean inicio2 = false;

    public void start(int port) {
        try {
            // Create the server socket
            serverSocket = new ServerSocket(port);
            System.out.println("Server started on port " + port);

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
            System.out.println("Client stopped");
        } catch (IOException e) {
            System.out.println("Error stopping server: " + e.getMessage());
        }
    }

    public void manage_message(String message) throws IOException {
        while (true) {
            try {
                // Espera a que un cliente se conecte
                clientSocket = serverSocket.accept();
                System.out.println("Cliente conectado desde " + clientSocket.getInetAddress().getHostName());

                // Crea los streams de entrada y salida para comunicarse con el cliente
                BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
                PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);

                // Maneja los mensajes del cliente
                String inputLine;
                while ((inputLine = in.readLine()) != null) {
                    // Si el mensaje es de tipo "A" (enviado desde el primer cliente)
                    if (inputLine.startsWith("A:")) {
                        if(inicio1 == false){
                            inicio1 = true;
                            Game game = new Game(serverSocket);
                            game.updateGame();
                            Thread.sleep(100);
                        }
                        String[] parts = inputLine.split(":");

                        // Maneja el mensaje de tipo "A"
                        System.out.println("Mensaje recibido del cliente 1: " + inputLine);
                        // Envia una respuesta al cliente
                        out.println("Respuesta al mensaje de tipo A: " + parts[2]);
                    }
                    // Si el mensaje es de tipo "B" (enviado desde el segundo cliente)
                    else if (inputLine.startsWith("B:")) {
                        if(inicio2 == false){
                            inicio2 = true;
                            //Game game2 = new Game(serverSocket);
                            //game2.updateGame();
                            Thread.sleep(100);
                        }
                        // Maneja el mensaje de tipo "B"
                        System.out.println("Mensaje recibido del cliente 2: " + inputLine);
                        // Envia una respuesta al cliente
                        out.println("Respuesta al mensaje de tipo B");
                    }
                    out.println("Mensaje erroneao recibido, verificar estructura");
                }
                stop();
            } catch (IOException | InterruptedException e) {
                System.out.println("Error al manejar la conexion con el cliente: " + e.getMessage());
            }
        }
    }


    public static void main(String[] args) throws IOException {
        Server server = new Server();
        server.start(8888);
        server.manage_message("sis");
    }
}