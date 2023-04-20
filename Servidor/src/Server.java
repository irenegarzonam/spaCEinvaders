import java.net.*;
import java.io.*;

public class Server {
    private static Game game1;
    private static Game game2;
    private ServerSocket serverSocket;
    private Socket clientSocket;
    private PrintWriter out;
    private BufferedReader in;
    private boolean inicio1 = false;
    private boolean inicio2 = false;
    private String datos;

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
                BufferedInputStream inputStream = new BufferedInputStream(clientSocket.getInputStream());
                PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);

                // Maneja los mensajes del cliente
                byte[] buffer = new byte[1024];
                int bytesRead;
                while ((bytesRead = inputStream.read(buffer)) != -1) {
                    String input = new String(buffer, 0, bytesRead);
                    // Si el mensaje es de tipo "A" Enviado solamente por Jugador1 o Espectador1
                    if (input.startsWith("A:")) {
                        //Verifica si ya hay alguien jugando
                        if(inicio1 == false){
                            inicio1 = true;
                            game1 = new Game(serverSocket);
                            datos = game1.updateGame();
                            Thread.sleep(100);
                        }
                        String[] parts = input.split(":"); //AQUI DIVIDE LO QUE RECIBE EL SERVER EN DIFERENTES STRINGS
                        datos = game1.updateGame();

                        // Maneja el mensaje de tipo "A"
                        System.out.println("Mensaje recibido del cliente 1: " + datos);
                        // Envia una respuesta al cliente
                        out.println(datos);
                    }
                    // Si el mensaje es de tipo "B" Enviado solamente por Jugador2 o Espectador2
                    else if (input.startsWith("B:")) {
                        if(inicio2 == false){
                            inicio2 = true;
                            //game2 = new Game(serverSocket);
                            //game2.updateGame();
                        }
                        String[] parts = input.split(":"); //AQUI DIVIDE LO QUE RECIBE EL SERVER EN DIFERENTES STRINGS
                        //game2.updateGame();
                        // Maneja el mensaje de tipo "B"

                        System.out.println("Mensaje recibido del cliente 2: " + input);
                        // Envia una respuesta al cliente
                        out.println("Muy bien, me sorprende, vamos crack!!!!!" + parts[0]);
                    } else {
                        out.println("Sea pendejo y arregle esto xdddd");
                    }
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