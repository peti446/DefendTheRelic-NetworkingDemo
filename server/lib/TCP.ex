defmodule TCP do
  @tcp_port 13002
  def port, do: @tcp_port

  def start_link() do
    options = [:binary, reuseaddr: true, active: :once, backlog: 5]
    case :gen_tcp.listen(@tcp_port, options) do
  		{:ok, socket} ->
  			pid = spawn(__MODULE__, :accept_loop, [socket])
        IO.puts("Started TCP server over the port #{inspect @tcp_port} and options #{inspect options}")
        {:ok, pid}
      {:error, reason} ->
  			IO.puts("Could not listen on tcp port #{inspect @tcp_port} with options #{inspect options}. Returned error: #{inspect reason}")
        {:error, "Error starting TCP server (Start listening)"}
    end
  end

  def accept_loop(listening_socket) do
    case :gen_tcp.accept(listening_socket) do
      {:ok, client_socket} ->
        IO.puts("Socket #{inspect client_socket} connected ok. Creating child process")

        case TcpSupervisor.add_child([client_socket]) do
          {:ok, pid} ->
            :gen_tcp.controlling_process(client_socket, pid)
            accept_loop(listening_socket)
        end
      {:error, reason} ->
        IO.puts("Could not accept socket reason: #{inspect reason}")
        throw("Error acceping connection")
    end
  end

##Client loops to accept messages
  def start_tcp_client_loop(client_socket) do
    pid = spawn(__MODULE__, :tcp_client_loop, [client_socket])
    {:ok, pid}
  end

	def tcp_client_loop(socket) do
    receive do
     {:tcp, socket, data} ->
         :inet.setopts(socket, [active: :once])
         IO.puts("Recived #{inspect data}")
         messageList = Utility.networkStringSplitter(Utility.packetToString(Utility.packetToString(data)));
         Router.server_handle_msg({socket, messageList})
         tcp_client_loop(socket)
     {:tcp_closed, socket} ->
          IO.puts("Socket closing #{inspect socket}")
     _ ->
          IO.puts("Could not handle msg")
    end
  end


  ##API to be accesed by the client
    def send_tpc_message(socket, msg) do
      :gen_tcp.send(socket, Utility.add_header_to_str(msg))
    end
end
