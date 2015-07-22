function acc = kth_train_LED_RBF(path,dim, delta)

gamma = 1/dim*2^delta;
RIEMANNIAN_KERNEL = @(X,Y,gamma) exp( -gamma*( norm(logm(X) - logm(Y),'fro') )^2 );


actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');
scale_factor = 1;
shift = 0;

n_actions = size(actions,1);
n_peo =  size(all_people,1);
sc = 1;

acc = [];

n_test = (n_peo-1)*n_actions;



load_sub_path =strcat(path, 'cov_matrices/kth-one-cov-mat-dim', int2str(dim), '/sc', int2str(sc), '/scale', int2str(scale_factor), '-shift', int2str(shift) );
for pe_ts= 1: n_peo
    X_train = zeros(dim,dim,n_test);
    labels_train = zeros(n_test,1);
    k =1;
    for pe_tr=1: n_peo
        if pe_tr~=pe_ts
            for act=1: n_actions
                name_load_cov = strcat( load_sub_path, '/cov_', all_people(pe_tr), '_', actions(act), '_dim', int2str(dim), '.h5');
                hinfo = hdf5info( char(name_load_cov) );
                one_video = hdf5read(hinfo.GroupHierarchy.Datasets(1));
                X_train(:,:,k) = one_video;
                labels_train(k) = act;
                k=k+1;               
            end
        end
    end
    
    
    K_train = compute_kernel_svm(X_train,X_train, RIEMANNIAN_KERNEL, gamma);
    model = svmtrain(labels_train, [[1:size(K_train,1)]' K_train], '-t 4 -q ');
    %Borrame Funciona Bien
    [predict_label, accuracy, dec_values] = svmpredict(labels_train,[[1:size(K_train,1)]' K_train], model);
    %display(accuracy');
    
    acc = [acc accuracy(1)];
    save_svm_model =strcat( './svm_models/LED-RBF_svm_run_', int2str(pe_ts), '_delta', num2str(delta),'.mat');
    save(save_svm_model, 'model', 'X_train');
    
    
end

