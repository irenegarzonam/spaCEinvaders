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


            while(true){

                clientSocket = serverSocket.accept();
                System.out.println("Cliente conectado: " + clientSocket);

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

                }

                in.close();
                out.close();
                clientSocket.close();
            }

        } catch (IOException e) {
            System.out.println("Error en la conexión: " + e.getMessage());
        }
    }
}