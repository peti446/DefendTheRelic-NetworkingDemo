defmodule GlobalSupervisor do
    use Supervisor

    def start_link(arg) do
      IO.puts("Starting Global Supervisor")
      Supervisor.start_link(__MODULE__, arg, name: __MODULE__)
    end

    def init(_args) do
      children = [
        worker(Router, [], restart: :transient),
        worker(UDP, [], restart: :transient),
		    worker(TCP, [], restart: :transient),
        supervisor(TcpSupervisor, [], restart: :transient, shutdown: :infinity)
      ]

      supervise(children, strategy: :one_for_one);
    end
end

defmodule TcpSupervisor do
  use Supervisor

  def start_link() do
    IO.puts("Starting TCP Client Supervisor")
    Supervisor.start_link(__MODULE__, [], name: __MODULE__);
  end

  def init(_args) do
    children = [
      worker(TCP, [], restart: :transient, function: :start_tcp_client_loop),
    ]
    supervise(children ,strategy: :simple_one_for_one, extra_arguments: [restart: :transient]);
  end

  def add_child(child_spec) do
    Supervisor.start_child(__MODULE__, child_spec);
  end

  def count_children() do
    Supervisor.count_children(__MODULE__)
  end
end
