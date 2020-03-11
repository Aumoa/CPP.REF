#pragma once

namespace SC::Game::Details
{
	class AsyncLoad : virtual public Object, virtual public IAsyncLoad
	{
		RefPtr<IAsyncOperation<object>> asyncLoad;
		double progress = 0;

	public:
		AsyncLoad()
		{

		}

		void Bind( RefPtr<IAsyncOperation<object>> asyncLoad )
		{
			this->asyncLoad = asyncLoad;
			this->asyncLoad->Canceled += EventHandler<>( this, &AsyncLoad::OnCanceled );
		}

		void Cancel() override
		{
			asyncLoad->Cancel();
		}

		void Close() override
		{
			asyncLoad->Close();
		}

		int64 Id_get() override
		{
			return asyncLoad->Id;
		}

		RefPtr<Exception> Error_get() override
		{
			return asyncLoad->Error;
		}

		Threading::AsyncStatus Status_get() override
		{
			return asyncLoad->Status;
		}

		object GetResults() override
		{
			return asyncLoad->GetResults();
		}

		RefPtr<IAsyncOperation<object>> Then( Threading::AsyncTaskChainDelegate<object> asyncTask, object args ) override
		{
			return asyncLoad->Then( asyncTask, args );
		}

		double Progress_get() override
		{
			return progress;
		}

		void Progress_set( double value )
		{
			progress = std::clamp<double>( value, 0, 1 );
		}

	private:
		void OnCanceled( object sender )
		{
			Canceled( sender );
		}
	};
}