// Copyright (c) 2017-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BLOCKNET_XBRIDGE_XBRIDGEWALLETCONNECTORETH_H
#define BLOCKNET_XBRIDGE_XBRIDGEWALLETCONNECTORETH_H

#include <xbridge/xbridgewalletconnector.h>

#include <string>
#include <vector>

//*****************************************************************************
//*****************************************************************************
namespace xbridge
{

/**
 * @brief Wallet connector for account-model (EVM) chains.
 *
 * STUB. Every method reports failure. This exists to establish the build
 * wiring and the registration point before any behaviour is written; a node
 * configured with method=ETH will start, construct this connector, and then
 * decline to do anything with it.
 *
 * The WalletConnector interface is shaped around the UTXO model, and roughly
 * two thirds of it has no account-model meaning: getUnspent, splitUtxos,
 * createDepositUnlockScript, getScriptId, isDustAmount and the transaction
 * builders that take XTxIn inputs. Those are marked NOT-APPLICABLE below and
 * are the subject of a separate design discussion, because the order manager
 * reaches past this interface and holds UtxoEntry values directly.
 *
 * The methods that DO map cleanly are marked MAPS. Two of them --
 * checkDepositTransaction and getSecretFromPaymentTransaction -- correspond
 * exactly to verifying an EtomicSwap lock and recovering the preimage from a
 * ReceiverSpent event.
 */
class EthWalletConnector : public WalletConnector
{
public:
    EthWalletConnector();

public:
    // MAPS: eth_blockNumber against the configured RPC endpoint.
    bool init() override;

public:
    std::string fromXAddr(const std::vector<unsigned char> & xaddr) const override;
    std::vector<unsigned char> toXAddr(const std::string & addr) const override;

public:
    // MAPS: the connector holds one address; no wallet-side generation.
    bool getNewAddress(std::string & addr) override;
    bool requestAddressBook(std::vector<wallet::AddressBookEntry> & entries) override;

    // MAPS: eth_blockNumber, eth_getBalance.
    bool getInfo(rpc::WalletInfo & info) const override;

    // NOT-APPLICABLE: an account has no unspent outputs. See the order-funding
    // discussion; a synthetic single-entry model is one option.
    bool getUnspent(std::vector<wallet::UtxoEntry> & inputs,
                    const std::set<wallet::UtxoEntry> & excluded) const override;

    // MAPS: eth_getBlockByHash / eth_getBlockByNumber.
    bool getBlock(const std::string & blockHash, std::string & rawBlock) override;
    bool getBlockHash(const uint32_t & block, std::string & blockHash) override;
    bool getBlockCount(uint32_t & blockCount) override;
    bool getTransactionsInBlock(const std::string & blockHash,
                                std::vector<std::string> & txids) override;

    // NOT-APPLICABLE in the UTXO sense; would report whether a reservation is
    // still backed by the account balance.
    bool getTxOut(wallet::UtxoEntry & entry) override;

    // MAPS: eth_sendRawTransaction.
    bool sendRawTransaction(const std::string & rawtx,
                            std::string & txid,
                            int32_t & errorCode,
                            std::string & message) override;

    // MAPS: personal_sign / ecrecover.
    bool signMessage(const std::string & address, const std::string & message,
                     std::string & signature) override;
    bool verifyMessage(const std::string & address, const std::string & message,
                       const std::string & signature) override;

    // MAPS: txpool_content.
    bool getRawMempool(std::vector<std::string> & txids) override;

public:
    // MAPS: 20-byte hex address with an optional 0x prefix.
    bool hasValidAddressPrefix(const std::string & addr) const override;
    bool isValidAddress(const std::string & addr) const override;

    // NOT-APPLICABLE: account chains have no dust threshold.
    bool isDustAmount(const double & amount) const override;

    // MAPS: secp256k1, same curve as Bitcoin.
    bool newKeyPair(std::vector<unsigned char> & pubkey,
                    std::vector<unsigned char> & privkey) override;
    std::vector<unsigned char> getKeyId(const std::vector<unsigned char> & pubkey) override;

    // NOT-APPLICABLE: no P2SH.
    std::vector<unsigned char> getScriptId(const std::vector<unsigned char> & script) override;
    std::string scriptIdToString(const std::vector<unsigned char> & id) const override;

    // NOT-APPLICABLE as written: gas is not a function of input and output
    // counts. Would need a gas-price path of its own.
    double minTxFee1(const uint32_t inputCount, const uint32_t outputCount) const override;
    double minTxFee2(const uint32_t inputCount, const uint32_t outputCount) const override;

    // MAPS: verify an EtomicSwap lock -- calldata, PaymentSent log,
    // payments() state and the recomputed paymentHash.
    bool checkDepositTransaction(const std::string & depositTxId,
                                 const std::string & destination,
                                 double & amount,
                                 uint64_t & p2shAmount,
                                 uint32_t & depositTxVout,
                                 const std::string & expectedScript,
                                 double & excessAmount,
                                 bool & isGood) override;

    // MAPS: recover the preimage from a ReceiverSpent log.
    bool getSecretFromPaymentTransaction(const std::string & paymentTxId,
                                         const std::string & depositTxId,
                                         const uint32_t & depositTxVOut,
                                         const std::vector<unsigned char> & hx,
                                         std::vector<unsigned char> & secret,
                                         bool & isGood) override;

    // MAPS: EtomicSwap lockTime is a uint64 unix timestamp.
    uint32_t lockTime(const char role) const override;
    bool acceptableLockTimeDrift(const char role, const uint32_t lckTime) const override;

    // NOT-APPLICABLE: there is no script. The equivalent commitment is the
    // contract address plus the ethPayment parameters.
    bool createDepositUnlockScript(const std::vector<unsigned char> & myPubKey,
                                   const std::vector<unsigned char> & otherPubKey,
                                   const std::vector<unsigned char> & xdata,
                                   const uint32_t lockTime,
                                   std::vector<unsigned char> & resultSript) override;

    // MAPS in intent, NOT in signature: these become ethPayment, senderRefund
    // and receiverSpend calls. The XTxIn inputs are meaningless here.
    bool createDepositTransaction(const std::vector<XTxIn> & inputs,
                                  const std::vector<std::pair<std::string, double> > & outputs,
                                  std::string & txId,
                                  uint32_t & txVout,
                                  std::string & rawTx) override;

    bool createRefundTransaction(const std::vector<XTxIn> & inputs,
                                 const std::vector<std::pair<std::string, double> > & outputs,
                                 const std::vector<unsigned char> & mpubKey,
                                 const std::vector<unsigned char> & mprivKey,
                                 const std::vector<unsigned char> & innerScript,
                                 const uint32_t lockTime,
                                 std::string & txId,
                                 std::string & rawTx) override;

    bool createPaymentTransaction(const std::vector<XTxIn> & inputs,
                                  const std::vector<std::pair<std::string, double> > & outputs,
                                  const std::vector<unsigned char> & mpubKey,
                                  const std::vector<unsigned char> & mprivKey,
                                  const std::vector<unsigned char> & xpubKey,
                                  const std::vector<unsigned char> & innerScript,
                                  std::string & txId,
                                  std::string & rawTx) override;

    // NOT-APPLICABLE: partial fills require divisible funding, which needs the
    // order-funding work first.
    bool createPartialTransaction(const std::vector<XTxIn> inputs,
                                  const std::vector<std::pair<std::string, double> > outputs,
                                  std::string & txId,
                                  std::string & rawTx) override;

    bool splitUtxos(CAmount splitAmount, std::string addr, bool includeFees,
                    std::set<wallet::UtxoEntry> excluded, std::set<COutPoint> utxos,
                    CAmount & totalSplit, CAmount & splitIncFees, int & splitCount,
                    std::string & txId, std::string & rawTx,
                    std::string & failReason) override;

    bool isUTXOSpentInTx(const std::string & txid, const std::string & utxoPrevTxId,
                         const uint32_t & utxoVoutN, bool & isSpent) override;
};

} // namespace xbridge

#endif // BLOCKNET_XBRIDGE_XBRIDGEWALLETCONNECTORETH_H
