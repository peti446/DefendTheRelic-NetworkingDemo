defmodule TCP do

  def start_link() do
    port = 13000
    options = [:binary, reuseaddr: true, active: :once, backlog: 5]
    case :gen_tcp.listen(port, options) do
  		{:ok, socket} ->
  			pid = spawn_link(__MODULE__, :accept_loop, [socket])
        IO.puts("Started TCP server over the port #{inspect port} and options #{inspect options}")
        {:ok, pid}
      {:error, reason} ->
  			IO.puts("Could not listen on tcp port #{inspect port} with options #{inspect options}. Returned error: #{inspect reason}")
        {:error, "Error starting TCP server (Start listening)"}
    end
  end

  def accept_loop(listening_socket) do
    case :gen_tcp.accept(listening_socket) do
      {:ok, client_socket} ->
        IO.puts("Socket #{inspect client_socket} connected ok. Creating child process")
        TcpSupervisor.add_child([client_socket])
        accept_loop(listening_socket)
      {:error, reason} ->
        IO.puts("Could not accept socket reason: #{inspect reason}")
        throw("Error acceping connection")
    end
  end

  def start_tcp_client_loop(client_socket) do
    {:ok, spawn_link(__MODULE__, :tcp_client_loop, [client_socket])}
  end

	def tcp_client_loop(socket) do
    IO.puts("Starting Reciveing over socket #{inspect socket}")
    receive do
     {:tcp, socket, data} ->
         :inet.setopts(socket, [active: :once])
         IO.puts("Recived #{inspect data}")
         tcp_client_loop(socket)
     {:tcp_closed, socket} ->
          IO.puts("Socket closing #{inspect socket}")
     _ ->
          IO.puts("Could not handle msg")
    end
  end
end
