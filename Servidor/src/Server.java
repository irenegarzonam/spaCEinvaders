import java.net.*;
import java.io.*;

public class Server {
    public static void main(String[] args) {
        ServerSocket serverSocket = null;
        Socket clientSocket = null;
        PrintWriter out = null;
        BufferedReader in = null;

        try {
            // Crear el socket del servidor
            serverSocket = new ServerSocket(12345);

            // Esperar a que un cliente se conecte
            System.out.println("Esperando a un cliente...");
            clientSocket = serverSocket.accept();
            System.out.println("Cliente conectado: " + clientSocket);

            while(true){

                // Crear el flujo de salida al cliente
                out = new PrintWriter(clientSocket.getOutputStream(), true);

                // Crear el flujo de entrada desde el cliente
                in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));

                String inputLine;
                // Leer los mensajes del cliente
                while ((inputLine = in.readLine()) != null) {
                    System.out.println("Mensaje recibido del cliente: " + inputLine);

                    // Enviar una respuesta al cliente
                    out.println("Respuesta desde el servidor: " + inputLine);

                    if (inputLine.equals("Bye.")) {
                        break;
                    }
                }
            }

        } catch (IOException e) {
            System.out.println("Error en la conexión: " + e.getMessage());
        } finally {
            try {
                // Cerrar los flujos y los sockets
                out.close();
                in.close();
                clientSocket.close();
                serverSocket.close();
            } catch (IOException e) {
                System.out.println("Error al cerrar los flujos y los sockets: " + e.getMessage());
            }
        }
    }
}
