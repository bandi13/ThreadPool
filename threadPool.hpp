#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <future>
typedef std::unique_ptr<boost::asio::io_service::work> asio_worker;

class ThreadPool {
	public:
		ThreadPool(size_t threads) :service(), working(new asio_worker::element_type(service)) {
			while(threads--)
			{
				auto worker = boost::bind(&boost::asio::io_service::run, &(this->service));
				g.add_thread(new boost::thread(worker));
			}
		}

		// add new work item to the pool
		template<class F, class... Args>
			std::shared_future<typename std::result_of<F(Args...)>::type> enqueue(F&& f, Args&&... args)
			{
				using packaged_task_t = std::packaged_task<typename std::result_of<F(Args...)>::type ()>;
				using shared_future_t = std::shared_future<typename std::result_of<F(Args...)>::type>;

				std::shared_ptr<packaged_task_t> task(new packaged_task_t(
							std::bind(std::forward<F>(f), std::forward<Args>(args)...)
							));
				auto res = shared_future_t(task->get_future());
				service.post(boost::bind(&packaged_task_t::operator(), task));
				return std::move(res);
			}

		~ThreadPool() {
			working.reset(); //allow run() to exit
			g.join_all();
			service.stop();
		}

	private:
		boost::asio::io_service service; //< the io_service we are wrapping
		asio_worker working;
		boost::thread_group g; //< need to keep track of threads so we can join them
};
